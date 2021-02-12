int main(int argc, char* argv[]) {
    if (std::string(argv[1]) == "-h"
        || std::string(argv[1]) == "--help"
        || argc > 3) {
        dump_usage(argv)
    }

    if (std::string(argv[1]) == "--dump-avail-actions") {
        std::cout << "Invoked!" << std::endl;
    }
    else {
        std::cout << "Unknown arg: " << argv[1] << std::endl;
    }
}

void dump_usage(argv) {
    std::cout << "Usage: " << argv[0] << " (specify only one arg):" << std::endl
              << "--dump-avail-actions (dumps available actions as JSON)" << std::endl;
              << std::endl;
}
    
