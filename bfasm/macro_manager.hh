#pragma once

#include <mutex>
#include <filesystem>
#include <map>
#include "macro_error_codes.hh"

namespace fs = std::filesystem;

class MacroObject;

class MacroManager {
    public:
        static MacroManager * get_instance();

        int compile(fs::path &main, bool compile_clean=false, bool use_bfmc_folder=false, fs::path bfmc_folder=fs::path(""), int max_threads=-1);

        int load_object(fs::path &include, MacroObject * dependent);

    private:
        static MacroManager * instance;
        static std::mutex instance_mutex;
        MacroManager();
        ~MacroManager();

        bool clean_bfmc_files;
        bool have_bfmc_folder;
        fs::path * bfmc_folder_path;
        int threads_max;

        MacroObject * main_object;
        std::map<std::string, MacroObject *> loaded_objects;
    
        int load_bfm_file(fs::path &include, MacroObject * dependent);
        void init_dependencies(MacroObject * mo, MacroObject * dependent);
};

MacroManager * MacroManager::instance = nullptr;
std::mutex MacroManager::instance_mutex = std::mutex();
