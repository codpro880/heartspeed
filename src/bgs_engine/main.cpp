#include <iostream>

#include "third_party/json.hpp"

void dump_usage(char* argv[]) {
    std::cout << "Usage: " << argv[0] << " (specify only one arg):" << std::endl
              << std::endl
              << "--dump-avail-actions (dumps available actions as JSON)" << std::endl
              << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1
        || std::string(argv[1]) == "-h"
        || std::string(argv[1]) == "--help"
        || argc > 3) {
        dump_usage(argv);
        exit(1);
    }

    if (std::string(argv[1]) == "--dump-avail-actions") {
        std::cout << "Invoked!" << std::endl;
    }
    else {
        std::cout << "Unknown arg: " << argv[1] << std::endl;
    }
}

