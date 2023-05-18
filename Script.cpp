#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include <vector>
#include <algorithm>

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
            if (command == "median_filter") {
                medianFilter();
                continue;
            }
            if (command == "xpm2_open") {
                clear_image_if_any();
                string filename;
                input >> filename;
                image = loadFromXPM2(filename);
                continue;
            }
            if (command == "xpm2_save") {
                 string filename;
                 input >> filename;
                 saveToXPM2(filename, image);
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
                Color& pixel = image -> at(x, y);
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
                Color& pixel = image -> at(x, y);
                int gray = (pixel.red() + pixel.green() + pixel.blue()) / 3;
                pixel.red() = gray;
                pixel.green() = gray;
                pixel.blue() = gray;
            }
        }
    }

    bool comparar(Color c1, Color c2) {
        if (c1.red() == c2.red() && c1.green() == c2.green() && c1.blue() == c2.blue()) {
            return true;
        }
        return false;
    }
    void Script::replace() {
        // Replace all (r1, g1, b1) by (r2, g2, b2)
        Color c1;
        Color c2;
        input >> c1 >> c2;
        int width = image->width();
        int height = image->height();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color& pixel = image -> at(x, y);
                if (comparar(pixel, c1)) {
                    pixel = c2;
                } 
            }
        }
    }
    void Script::fill() {
        int x, y, w, h;
        Color c;

        input >> x >> y >> w >> h >> c;

        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                int posX = x + i;
                int posY = y + j;

                Color& pixel = image-> at(posX, posY);
                pixel = c;
            }
        }
    }
    void Script::hMirror() {
        int width = image->width();
        int height = image->height();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width / 2; x++) {
                Color& left_pixel = image -> at(x, y);
                Color& right_pixel = image -> at(width - 1 - x, y);
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
                Color& top_pixel = image-> at(x, y);
                Color& bottom_pixel = image -> at(x, height - 1 - y);
                swap(top_pixel.red(), bottom_pixel.red());
                swap(top_pixel.green(), bottom_pixel.green());
                swap(top_pixel.blue(), bottom_pixel.blue());
            }
        }
    }
    void Script::add() {
        string filename;
        int x, y;
        Color c;
        input >> filename >> c >> x >> y;

        Image* addedImage = loadFromPNG(filename);
        if (addedImage != nullptr && image != nullptr) {
            int addedWidth = addedImage->width();
            int addedHeight = addedImage->height();
            int imageWidth = image->width();
            int imageHeight = image->height();

            for (int j = 0; j < addedHeight; j++) {
                for (int i = 0; i < addedWidth; i++) {
                    Color& srcPixel = addedImage -> at(i, j);
                    int destX = x + i;
                    int destY = y + j;
                    if (destX >= 0 && destX < imageWidth && destY >= 0 && destY < imageHeight) {
                        Color& destPixel = image -> at(destX, destY);
                        if (!comparar(srcPixel, c)) {
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
                    Color& srcPixel = image -> at(x + i, y + j);
                    Color& destPixel = (*croppedImage).at(i, j);
                    destPixel = srcPixel;
                }
            }
            delete image;
            image = croppedImage;
        }
    }
    void Script::rotateRight() {
        int width = image->width();
        int height = image->height();
        Image* rotatedImage = new Image(height, width);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color& srcPixel = image -> at(x, y);
                Color& destPixel = rotatedImage -> at(height - 1 - y, x);
                destPixel = srcPixel;
            }
        }
        delete image;
        image = rotatedImage;
    }
    void Script::rotateLeft() {
        int width = image->width();
        int height = image->height();
        Image* rotatedImage = new Image(height, width);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color& srcPixel = image -> at(x, y);
                Color& destPixel = rotatedImage -> at(y, width - 1 - x);
                destPixel = srcPixel;
            }
        }
        delete image;
        image = rotatedImage;
    }

    void Script::medianFilter() {
        int width = image -> width();
        int height = image -> height();
        int ws;
        input>>ws;
        Image* filteredImage = new Image(width, height);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int startX = x - ws/2;
                int endX = x + ws/2;
                int startY = y - ws/2;
                int endY = y + ws/2;

                startX = max(0, startX);
                endX = min(endX, width - 1);
                startY = max(0, startY);
                endY = min(endY, height - 1);

                vector<Color> vizinhos;
                for (int ny = startY; ny <= endY; ny++) {
                    for (int nx = startX; nx <= endX; nx++) {
                        vizinhos.push_back(image -> at(nx, ny));
                    }
                }
                filteredImage->at(x, y) = Color :: Median(vizinhos);
            }
        }
        delete image;
        image = filteredImage;
        
    }
    

    

}
