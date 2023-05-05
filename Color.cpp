#include "Color.hpp"

namespace prog {
    Color::Color() {

    }
    Color::Color(const Color& other) : red_(other.red_), green_(other.green_), blue_(other.blue_) {}
    
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) : red_(red), green_(green), blue_(blue) {}
    
    rgb_value Color::red() const {
        return 0;
    }
    
    rgb_value Color::green() const {
        return 0;
    }
    
    rgb_value Color::blue() const {
        return 0;
    }

    // Usando os private fields red_, green_ e blue_
    rgb_value& Color::red()  {
        return red_;
    }
    rgb_value& Color::green()  {
      return green_;
    }
    rgb_value& Color::blue()  {
      return blue_;
    }
}
