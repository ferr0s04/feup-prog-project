#include "Image.hpp"
#include <iostream>
#include <vector>


namespace prog
{
  Image::Image(int w, int h, const Color &fill) : width_(w), height_(h) {
      data_.resize(height_, std::vector<Color>(width_));          
      for (int i = 0; i < height_; i++) {
        for (int j = 0; j < width_; j++) {
            data_[i][j] = fill;
        }
      }
  }

  Image::~Image() {}

  int Image::width() const {
    return width_;                   // Retirar a largura da imagem
  }

  int Image::height() const {
    return height_;                  // Retirar a altura
  }


  Color& Image::at(int x, int y) {
      return data_[y][x];                                           // Se os parâmetros da função estiverem dentro dos limites, então é devolvida a cor do pixel definido por esses parâmetros
  }

  const Color& Image::at(int x, int y) const {
      return data_[y][x];                                           // A mesma coisa, mas para leitura apenas
  }
}
