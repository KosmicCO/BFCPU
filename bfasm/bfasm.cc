#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/parsers.hpp>
#include <string>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char ** argv) {
    po::options_description desc("Compiler Options");
    desc.add_options()
        ("help,h", "Produce help message")
        ("macro,m", po::value<std::string>(), "Compiles given file only with macro syntax (.bfm)")
        ("asm,a", po::value<std::string>(), "Compiles given file with bf assembly (.bfa)")
        ("file-out,f", po::value<std::string>()->default_value(""), "Name of the output file (default: the target and .bfcpu)")
        ("macro-objects-dir,o", po::value<std::string>()->default_value(""), "Optional directory to put .bfmc files");

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
            options(desc).run(), vm);
    po::notify(vm);

    if(vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if(!((0 < vm.count("macro")) ^ (0 < vm.count("asm")))) {
        std::cout << "Need to specify compiling with either \"--macro\" or \"--asm\"" << std::endl;
        return 1;
    }

    if(vm.count("macro")) {
        // TODO: run macros compiler
        return 0;
    }

    if(vm.count("asm")) {
        // TODO: run assembler
        return 0;
    }

    std::cout << "Need to specify compiling with either \"--macro\" or \"--asm\"" << std::endl;
    return 1;
}
