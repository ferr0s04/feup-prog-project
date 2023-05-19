#include "Color.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

namespace prog {
    Color::Color() : red_(0), green_(0), blue_(0) {} // Construtor padrão que inicializa os componentes de cor como 0

    Color::Color(const Color& other) : red_(other.red_), green_(other.green_), blue_(other.blue_) {}  // Construtor de cópia que copia os valores das cores de outro objeto também Color

    Color::Color(rgb_value red, rgb_value green, rgb_value blue) : red_(red), green_(green), blue_(blue) {} // Construtor que aceita os valores das cores como argumentos e os atribui às variáveis correspondentes

    rgb_value Color::red() const { //Método getter para a cor vermelha
        return red_;
    }
    rgb_value Color::green() const { //Método getter para a cor verde
        return green_;
    }
    rgb_value Color::blue() const { //Método getter para a cor azul
        return blue_;
    }

    // Usando os private fields:

    rgb_value& Color::red()  { //Método setter para a cor vermelha
        return red_;
    }
    rgb_value& Color::green()  { //Método setter para a cor verde
      return green_;
    }
    rgb_value& Color::blue()  { //Método setter para a cor azul
      return blue_;
    }

    Color Color :: Median(const vector<Color>& v) { Calcula a cor mediana de um vetor de cores
        Color newC; // Cria um novo objeto para a cor mediana
        vector<rgb_value> vColor; // Vetor temporário para armazenar os valores de cor

        std::cout << "aqui";

        for (int i = 0; i < int(v.size()); i++) { //Loop que adiciona o valor da cor vermelha de cada cor ao vetor
            vColor.push_back(v[i].red());
        }
        sort(vColor.begin(), vColor.end()); //Ordena o vetor em ordem crescente

        if (vColor.size() % 2 != 0) {
            newC.red() = vColor[vColor.size() / 2];
        } else {
            newC.red() = (vColor[(vColor.size() / 2) - 1] + vColor[vColor.size() / 2]) / 2;
        }
        // É calculado a cor mediana para a cor vermelha
        vColor.clear(); //limpa o vetor temporário

        for (int i = 0; i < int(v.size()); i++) { //Loop que adiciona o valor da cor verde de cada cor ao vetor
            vColor.push_back(v[i].green());
        }
        sort(vColor.begin(), vColor.end()); //Ordena o vetor em ordem crescente

        if (vColor.size() % 2 != 0) {
            newC.green() = vColor[vColor.size() / 2];
        } else {
            newC.green() = (vColor[(vColor.size() / 2) - 1] + vColor[vColor.size() / 2]) / 2;
        }
        // É calculado a cor mediana para a cor verde
        vColor.clear(); //limpa o vetor temporário

        for (int i = 0; i < int(v.size()); i++) { //Loop que adiciona o valor da cor azul de cada cor ao vetor
            vColor.push_back(v[i].blue());
        }
        sort(vColor.begin(), vColor.end()); //Ordena o vetor em ordem crescente

        if (vColor.size() % 2 != 0) {
            newC.blue() = vColor[vColor.size() / 2];
        } else {
            newC.blue() = (vColor[(vColor.size() / 2) - 1] + vColor[vColor.size() / 2]) / 2;
        }
        // É calculado a cor mediana para a cor azul
        return newC; // retorna a cor mediana na forma (r,g,b)
    }
}
