#include "Color.hpp"

namespace prog {
    Color::Color() {

    }
    Color::Color(const Color& other) {

    }
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) : red_(red), green_(green), blue_(blue) {}
    
    rgb_value Color::red() const {
        return red_;
    }
    
    rgb_value Color::green() const {
        return green_;
    }
    
    rgb_value Color::blue() const {
        return blue_;
    }

    // TODO: remove this DUMMY variable once you have appropriate fields for representing colors.
    rgb_value DUMMY_rgb_value = 0; 

    rgb_value& Color::red()  {
        return DUMMY_rgb_value;
    }
    rgb_value& Color::green()  {
      return DUMMY_rgb_value;
    }
    rgb_value& Color::blue()  {
      return DUMMY_rgb_value;
    }
}
