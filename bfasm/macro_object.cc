#include "macro_object.hh"

MacroObject::MacroObject(fs::path file) :
    relative_path(nullptr),
    bfm_path(nullptr),
    bfmc_path(nullptr),
    raw_file(nullptr),
    error(false),
    dependents(),
    dependencies(),
    unaliased_deps(),
    aliased_deps(),
    local_functions_order(),
    local_functions_set(),
    unresolved_references(),
    local_function_def(),
    error_queue()
{
    if(file.extension().compare(".bfm") == 0) {
        bfm_path = new fs::path(file);
    } else if(file.extension().compare(".bfmc") == 0) {
        bfmc_path = new fs::path(file);
    } else {
        throw std::runtime_error("File of neither .bfm nor .bfmc extension passed to MacroObject Constructor");
    }

    relative_path = new fs::path(file.parent_path());
}
