#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using std::cout;
namespace fs = std::filesystem;

bool is_jpeg(const unsigned char* buf);
bool is_png(const unsigned char* buf);

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
        return 1;
    }

    std::string imagefile = argv[1];
    cout << "File: " << imagefile << "\n";

    if (!fs::is_regular_file(imagefile)) {
        std::cout << "File not found: " << imagefile << "\n";
        return 1;
    }

    std::ifstream file(imagefile, std::ios::binary);
    if (!file) {
        cout << "Cannot open file!\n";
        return 1;
    }

    unsigned char header[8];
    file.read(reinterpret_cast<char*>(header), 8);

    if (is_png(header)) {
        cout << "PNG detected\n";
    } else if (is_jpeg(header)) {
        cout << "JPEG detected\n";
    } else {
        cout << "Unkown format!\n";
        return 1;
    }

    int width, height, channels;

    unsigned char* data = stbi_load(
        argv[1],
        &width,
        &height,
        &channels,
        3 //forcing to RGB
        );

    if (!data) {
        cout << "Failed to load image\n";
        return 1;
    }

    cout << "Loaded image:\n";
    cout << "Width: " << width << "\n";
    cout << "Height: " << height << "\n";
    cout << "Channels: " << channels << "\n";

    stbi_image_free(data);

    return 0;
}

bool is_jpeg(const unsigned char* buf) {
    return buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF;
}

bool is_png(const unsigned char* buf) {
    return buf[0] == 0x89 &&
           buf[1] == 0x50 &&
           buf[2] == 0x4E &&
           buf[3] == 0x47 &&
           buf[4] == 0x0D &&
           buf[5] == 0x0A &&
           buf[6] == 0x1A &&
           buf[7] == 0x0A;
}
