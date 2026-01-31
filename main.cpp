#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>
#include <unordered_map>
#include <algorithm>

using std::cout;
namespace fs = std::filesystem;

bool is_jpeg(const unsigned char* buf);
bool is_png(const unsigned char* buf);

struct Pixel {
    unsigned char r, g, b;
};

struct Bucket {
    long r = 0;
    long g = 0;
    long b = 0;
    int count = 0;
};

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

    std::vector<Pixel> samples;

    int step = 10;          //downsample rate
    for (int y = 0; y< height; y += step) {
        for (int x = 0; x < width; x+= step) {
            int idx = (y * width + x) * 3;

            Pixel p;
            p.r = data[idx];
            p.g = data[idx + 1];
            p.b = data[idx + 2];
            samples.push_back(p);
        }
    }

    cout << "Sampled pixels: " << samples.size() << "\n";

    std::unordered_map<int, Bucket> hist;

    for (auto& p : samples) {
        int brightness = p.r + p.g + p.b;
        if (brightness < 60) continue;           //filtering out dark pixels

        int minc = std::min({p.r, p.g, p.b});
        int maxc = std::max({p.r, p.g, p.b});
        if (maxc - minc < 10) continue;         //filtering out low saturation
                                                //(gray) colors

        int rq = p.r / 16;
        int gq = p.g / 16;
        int bq = p.b / 16;

        int key = (rq << 8) | (gq << 4) | bq;

        auto& bucket = hist[key];
        bucket.r += p.r;
        bucket.g += p.g;
        bucket.b += p.b;
        bucket.count ++;
    }

    std::vector<Bucket> buckets;

    for (auto& kv : hist) {
        buckets.push_back(kv.second);
    }

    std::sort(buckets.begin(), buckets.end(),
            [](const Bucket& a, const Bucket& b) {
                return a.count > b.count;
            });

    int paletteSize = std::min(5, (int)buckets.size());

    for (int i = 0; i < paletteSize; i++) {
        auto& b = buckets[i];
        int r = b.r / b.count;
        int g = b.g / b.count;
        int bcol = b.b / b.count;

        printf("#%02X%02X%02X\n", r, g, bcol);
    }

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
