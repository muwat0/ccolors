#include <iostream>
#include <string>
#include <filesystem>
using std::cout;
namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    for (int i = 0; i <= argc - 1; i++) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            cout << "help flag detected!";
            return 0;
        }
    }

    if (argc != 2) {
        cout << "Please specify image file. " << argv[0] << " imagefile.jpg";
    } else {
        std::string imagefile = argv[1];
        cout << "File: " << imagefile << "\n";

        if (!fs::is_regular_file(imagefile)) {
            std::cout << "File not found: " << imagefile << "\n";
            return 1;
        }
    }

    return 0;
}

