#pragma once

#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <thread>
#include <mutex> 
#include <filesystem>
#include <memory>
#include "macro_manager.hh"

namespace fs = std::filesystem;

class MacroObject {
    public:
        MacroObject(fs::path file);
        ~MacroObject();

        int load_files();
        int check_syntax();             // checks to make sure basic syntax use is preserved
        int load_dependencies();        // loads dependency list and line number and check for validity
        int generate_local_func_list(); // generates the list and set of local function definitions
        int check_linking();            // checks to see if references exist for non-local function references
        int generate_bfmc();            // compile

        void register_dependent(MacroObject * dep);
        bool is_error();
        std::shared_ptr<std::queue<std::string>> get_errors();

    private:
        fs::path * relative_path;
        fs::path * bfm_path;
        fs::path * bfmc_path;

        std::stringstream raw_file;

        bool error;

        std::vector<MacroObject *> dependents;
        std::vector<MacroObject *> dependencies;

        std::vector<MacroObject *> unaliased_deps;
        std::map<std::string, MacroObject *> aliased_deps;

        std::vector<std::string> local_functions_order;
        std::set<std::string> local_functions_set;

        std::set<std::string> unresolved_references;
        std::map<std::string, std::stringstream *> local_function_def;

        std::shared_ptr<std::queue<std::string>> error_queue;


        void set_error() { error = true; }
};
