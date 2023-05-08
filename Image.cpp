#include "Image.hpp"
#include <stdexcept>

using namespace std;

namespace prog
{
  Image::Image(int w, int h, const Color &fill) : width_(w), height_(h) {
      data_ = new Color*[height_];                // Cria um novo array de elementos do tipo "Color" de tamanho "height_"
      for (int i = 0; i < height_; i++) {
          data_[i] = new Color[width_];           // A cada elemento desse novo array, cria outro novo com elementos do tipo "Color" de tamanho "width"
      }
      for (int i = 0; i < height_; i++) {
          for (int j = 0; j < width_; j++) {
              data_[i][j] = fill;                 // Definir a cor de cada pixel com a cor de "fill", através do array criado
          }
      }
  }

  Image::~Image() {
      for (int i = 0; i < height_; i++) {
          delete [] data_[i];                  // Acede a cada array dentro do array principal e apaga-o
      }
      delete [] data_;                         // Agora apaga o array principal
  }

  int Image::width() const {
    return width_;                   // Retirar a largura da imagem
  }

  int Image::height() const {
    return height_;                  // Retirar a altura
  }


  Color& Image::at(int x, int y) {
      if (!(x >= 0 && x < width_ && y >= 0 && y < height_)) {
          throw out_of_range("Invalid pixel coordinates");          // Para quando os parâmetros estiverem fora dos limites
      }
      return data_[x][y];                                           // Se os parâmetros da função estiverem dentro dos limites, então é devolvida a cor do pixel definido por esses parâmetros
  }

  const Color& Image::at(int x, int y) const {
      if (!(x >= 0 && x < width_ && y >= 0 && y < height_)) {
          throw out_of_range("Invalid pixel coordinates");
      }
      return data_[x][y];                                           // A mesma coisa, mas para leitura apenas
  }
}
