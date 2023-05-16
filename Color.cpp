#include "Color.hpp"

namespace prog {
    Color::Color() : red_(0), green_(0), blue_(0) {} // Default constructor = (0, 0, 0)
    
    Color::Color(const Color& other) : red_(other.red_), green_(other.green_), blue_(other.blue_) {} // Copy constructor
    
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) : red_(red), green_(green), blue_(blue) {}
    
    // Funções apenas para leitura
    rgb_value Color::red() const {
        return red_;
    }
    
    rgb_value Color::green() const {
        return green_;
    }
    
    rgb_value Color::blue() const {
        return blue_;
    }

    // Funções para leitura e escrita
    rgb_value& Color::red()  {
        return red_;
    }
    rgb_value& Color::green()  {
      return green_;
    }
    rgb_value& Color::blue()  {
      return blue_;
    }
    
    Color Color :: Median(const vector<Color>& v) {
        Color newC;
        vector<rgb_value> vColor;
        for (int i = 0; i < v.size(); i++) {
            vColor.push_back(v[i].red());
        }
        sort(vColor.begin(), vColor.end());
        if (vColor.size() % 2 != 0) {
            newC.red() = vColor[vColor.size() / 2];
        } else {
            newC.red() = (vColor[(vColor.size() / 2) - 1] + vColor[vColor.size() / 2]) / 2;
        }

        vColor.clear();

        for (int i = 0; i < v.size(); i++) {
            vColor.push_back(v[i].green());
        }
        sort(vColor.begin(), vColor.end());
        if (vColor.size() % 2 != 0) {
            newC.green() = vColor[vColor.size() / 2];
        } else {
            newC.green() = (vColor[(vColor.size() / 2) - 1] + vColor[vColor.size() / 2]) / 2;
        }

        vColor.clear();

        for (int i = 0; i < v.size(); i++) {
            vColor.push_back(v[i].blue());
        }
        sort(vColor.begin(), vColor.end());
        if (vColor.size() % 2 != 0) {
            newC.blue() = vColor[vColor.size() / 2];
        } else {
            newC.blue() = (vColor[(vColor.size() / 2) - 1] + vColor[vColor.size() / 2]) / 2;
        }

        return newC;
    }
}
