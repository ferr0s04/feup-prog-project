#include "Image.hpp"
#include <iostream>
#include <vector>


namespace prog
{
    Image::Image(int w, int h, const Color &fill) : width_(w), height_(h) { // construtor da classe Image que aceita o comprimento e a largura da imagem, além da cor para preencher a imagem como argumentos
        data_.resize(height_, std::vector<Color>(width_));
        // Redimensiona o vetor data_ para ter height_ elementos, onde cada elemento é do tipo Color e um vetor  width_ --> cria uma matriz de pixels para armazenar os dados da imagem
        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                data_[i][j] = fill; // preenche cada pixel com uma cor em específico
            }
        }
    }

    Image::~Image() {} // Destrutor da classe Image.

    int Image::width() const {
        return width_; // Retirar a largura da imagem
    }

    int Image::height() const {
        return height_; // Retirar a altura da imagem
    }


    Color& Image::at(int x, int y) {
        return data_[y][x];// Retorna uma referência para o pixel na posição (x,y) da iamgem, permitindo a sua modificação 
    }

    const Color& Image::at(int x, int y) const {
        return data_[y][x];     // Retorna uma referência para o pixel na posição (x,y) da iamgem, servindo apenas para a sua leitura
        
    }
}
