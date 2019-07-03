#include "../headers/utils.h"
#include <vector>
namespace PNGImage {
    void read(const std::string &imgName, int &height, int &width, Vec3** &img) {
        std::vector<unsigned char> image;
        unsigned h, w;
        unsigned error_code = lodepng::decode(image, w, h, imgName.c_str());
        height = h;
        width = w;
        if(error_code)
            throw lodepng_error_text(error_code);
        img = new Vec3* [h];
        for(size_t i = 0; i < h; i++)
            img[i] = new Vec3[w];
        for(size_t i = 0; i < h; i++)
            for(size_t j = 0; j < w; j++) {
                img[i][j] = Vec3(image[((i * w + j) << 2)], image[((i * w + j) << 2) | 1], image[((i * w + j) << 2) | 2]);
                img[i][j] /= 255.0;
            }
    }
    void write(const std::string &imgName, int height, int width, Vec3 **img) {
        std::vector<unsigned char>image;
        unsigned h = height, w = width;
        for(size_t i = 0; i < h; i++)
            for(size_t j = 0; j < w; j++) {
                image.push_back((unsigned char)(int)(img[i][j][0] * 255));
                image.push_back((unsigned char)(int)(img[i][j][1] * 255));
                image.push_back((unsigned char)(int)(img[i][j][2] * 255));
                image.push_back((unsigned char)255);
            }
        unsigned error_code = lodepng::encode(imgName.c_str(), image, w, h);
        if(error_code)
            throw lodepng_error_text(error_code);
    }
}
