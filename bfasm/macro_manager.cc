#include "macro_manager.hh"
#include "macro_object.hh"

MacroManager::MacroManager() :
    clean_bfmc_files(false),
    have_bfmc_folder(false),
    bfmc_folder_path(nullptr),
    threads_max(0),
    main_object(nullptr),
    loaded_objects()
{

}

void MacroManager::init_dependencies(MacroObject * mo, MacroObject * dependent) {
    if(mo->load_files() == 0) {
        if(mo->check_syntax() == 0) {
            mo->load_dependencies();
        }
    }
    if(!mo->is_error()) {
        mo->register_dependent(dependent);
    }
}

int MacroManager::load_bfm_file(fs::path &include, MacroObject * dependent) {
    if(is_regular_file(include)) {
        MacroObject * mo = new MacroObject(include);
        loaded_objects.insert({include.string(), mo});
        init_dependencies(mo, dependent);
        return 0;
    }
    return ERR_BFM_FILE_NOT_FOUND;
}

int MacroManager::load_object(fs::path &include, MacroObject * dependent) {
    if(include.extension().compare(".bfm") != 0) {
        return ERR_IMPROPER_IMPORT_FILE;
    }
    fs::path bfm_include = fs::canonical(include);
    if(loaded_objects.count(bfm_include.string()) <= 0) {
        fs::path bfmc_include(bfm_include);
        bfmc_include.replace_extension(".bfmc");
        if(is_regular_file(bfmc_include)) {
            if(clean_bfmc_files) {
                if(remove(bfmc_include.string().c_str()) != 0) {
                    return ERR_UNABLE_CLEAR_BFMC;
                } else {
                    return load_bfm_file(bfm_include, dependent);
                }
            } else {
                MacroObject * mo = new MacroObject(bfmc_include);
                loaded_objects.insert({bfm_include.string(), mo});
                init_dependencies(mo, dependent);
                return 0;
            }
        } else {
            return load_bfm_file(bfm_include, dependent);
        }
    } else {
        loaded_objects.find(bfm_include.string())->second->register_dependent(dependent);
        return 0;
    }
}

int MacroManager::compile(fs::path &main, bool compile_clean, bool use_bfmc_folder, fs::path bfmc_folder, int max_threads) {
    clean_bfmc_files = compile_clean;                           // initialize values
    have_bfmc_folder = use_bfmc_folder;
    bfmc_folder_path = new fs::path(bfmc_folder);
    threads_max = max_threads;

    fs::path canon_main = canonical(main);                      // initialize main_object

    if(!canon_main.extension().compare(".bfm")) {
        return ERR_IMPROPER_MAIN_FILE;
    }

    fs::path canon_main_bfmc(canon_main);
    canon_main_bfmc.replace_extension(".bfmc");
    if(is_regular_file(canon_main_bfmc)) {
        if(remove(canon_main_bfmc.string().c_str()) != 0) {
            return ERR_UNABLE_CLEAR_BFMC;
        }
    }
    if(!is_regular_file(canon_main)) {
        return ERR_BFM_FILE_NOT_FOUND;
    }

    main_object = new MacroObject(canon_main);                  // start recursive dependency find
    loaded_objects.insert({canon_main.string(), main_object});
    if(main_object->load_files() == 0) {
        if(main_object->check_syntax() == 0) {
            main_object->load_dependencies();
        }
    }
    
    // TODO: check main_object.is_error()


    return 0;
}

MacroManager * MacroManager::get_instance() {
    if(instance == nullptr) {
        std::scoped_lock lock(instance_mutex);
        if(instance == nullptr) {
            instance = new MacroManager();
        }
    }

    return instance;
}
