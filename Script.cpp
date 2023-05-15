#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            if (command == "invert") {
                invert();
                continue;
            }
            if (command == "to_gray_scale") {
                toGrayScale();
                continue;
            }
            if (command == "replace") {
                replace();
                continue;
            }
            if (command == "fill") {
                fill();
                continue;
            }
            if (command == "h_mirror") {
                hMirror();
                continue;
            }
            if (command == "v_mirror") {
                vMirror();
                continue;
            }
            if (command == "add") {
                add();
                continue;
            }
            if (command == "crop") {
                crop();
                continue;
            }
            if (command == "rotate_left") {
                rotateLeft();
                continue;
            }
            if (command == "rotate_right") {
                rotateRight();
                continue;
            }

        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
    void Script::invert() {
        // Each pixel (r, g, b) is transformed to (255 - r, 255 - g, 255 - b)
        int width = image->width();
        int height = image->height();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color& pixel = (*image).at(x, y);
                pixel.red() = 255 - pixel.red();
                pixel.green() = 255 - pixel.green();
                pixel.blue() = 255 - pixel.blue();
        }
    }
    }
    void Script::toGrayScale() {
        // Each pixel (r, g, b) is transformed to (v, v, v), where v = (r + g + b)/3
        int width = image->width();
        int height = image->height();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color& pixel = (*image).at(x, y);
                int gray = (pixel.red() + pixel.green() + pixel.blue()) / 3;
                pixel.red() = gray;
                pixel.green() = gray;
                pixel.blue() = gray;
        }
    }
    }
    void Script::replace() {
        // Replace all (r1, g1, b1) by (r2, g2, b2)
        int r1, g1, b1, r2, g2, b2;
        input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
        int width = image->width();
        int height = image->height();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color& pixel = (*image).at(x, y);
                if (pixel.red() == r1 && pixel.green() == g1 && pixel.blue() == b1) {
                    pixel.red() = r2;
                    pixel.green() = g2;
                    pixel.blue() = b2;
            }
        }
    }
    }
    void Script::fill() {
        int x, y, w, h, r, g, b;
        input >> x >> y >> w >> h >> r >> g >> b;

        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                int posX = x + i;
                int posY = y + j;

                Color& pixel = (*image).at(posX, posY);
                pixel.red() = r;
                pixel.green() = g;
                pixel.blue() = b;
        }
    }
    }
    void Script::hMirror() {
        int width = image->width();
        int height = image->height();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width / 2; x++) {
                Color& left_pixel = (*image).at(x, y);
                Color& right_pixel = (*image).at(width - 1 - x, y);
                swap(left_pixel.red(), right_pixel.red());
                swap(left_pixel.green(), right_pixel.green());
                swap(left_pixel.blue(), right_pixel.blue());
        }
    }
    }
    void Script::vMirror() {
       int width = image->width();
        int height = image->height();

        for (int y = 0; y < height / 2; y++) {
            for (int x = 0; x < width; x++) {
                Color& top_pixel = (*image).at(x, y);
                Color& bottom_pixel = (*image).at(x, height - 1 - y);
                swap(top_pixel.red(), bottom_pixel.red());
                swap(top_pixel.green(), bottom_pixel.green());
                swap(top_pixel.blue(), bottom_pixel.blue());
        }
    } 
    }
    void Script::add() {
        string filename;
        int r, g, b, x, y;
        input >> filename >> r >> g >> b >> x >> y;

        Image* addedImage = loadFromPNG(filename);
        if (addedImage != nullptr && image != nullptr) {
            int addedWidth = addedImage->width();
            int addedHeight = addedImage->height();
            int imageWidth = image->width();
            int imageHeight = image->height();

            for (int j = 0; j < addedHeight; j++) {
                for (int i = 0; i < addedWidth; i++) {
                    Color& srcPixel = (*addedImage).at(i, j);
                    int destX = x + i;
                    int destY = y + j;
                    if (destX >= 0 && destX < imageWidth && destY >= 0 && destY < imageHeight) {
                        Color& destPixel = (*image).at(destX, destY);
                        if (srcPixel.red() != r || srcPixel.green() != g || srcPixel.blue() != b) {
                            destPixel = srcPixel;
                    }
                }
            }
        }

        delete addedImage;
    }
    }
    void Script::crop() {
        int x, y, w, h;
        input >> x >> y >> w >> h;
        if (image != nullptr) {
            int imageWidth = image->width();
            int imageHeight = image->height();
            x = max(x, 0);
            y = max(y, 0);
            w = min(w, imageWidth - x);
            h = min(h, imageHeight - y);
            Image* croppedImage = new Image(w, h);
            for (int j = 0; j < h; j++) {
                for (int i = 0; i < w; i++) {
                    Color& srcPixel = (*image).at(x + i, y + j);
                    Color& destPixel = (*croppedImage).at(i, j);
                    destPixel = srcPixel;
            }
        }
        delete image;
        image = croppedImage;
    }
    }
    void Script::rotateLeft() {
        int width = image->width();
        int height = image->height();
        Image* rotatedImage = new Image(height, width);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color& srcPixel = (*image).at(x, y);
                Color& destPixel = (*rotatedImage).at(height - 1 - y, x);
                destPixel = srcPixel;
            }
        }
        delete image;
        image = rotatedImage;
    }
    void Script::rotateRight() {
        int width = image->width();
        int height = image->height();
        Image* rotatedImage = new Image(height, width);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color& srcPixel = (*image).at(x, y);
                Color& destPixel = (*rotatedImage).at(y, width - 1 - x);
                destPixel = srcPixel;
            }
        }
        delete image;
        image = rotatedImage;
    }
}
