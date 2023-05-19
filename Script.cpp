#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include <vector>
#include <algorithm>

using namespace std;

namespace prog {
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
        // usado para ler os valores da cor diretamente do ficheiro
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
        // usado para eliminar a imagem atual antes de executar outras funções
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            // estes comandos não requerem uma imagem já aberta.
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // estes comandos requerem que uma imagem esteja já aberta.
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
                clear_image_if_any(); // nesta função é recriada uma imagem a partir de xmp2 pelo que é necessário eliminar a imagem atual
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

        //é a função responsável por receber os comandos a serem executados e encaminhando a partir de cada instrução para a função correspondente ao comando
        }
    }
    void Script::open() {
        clear_image_if_any(); // elimina a imagem atual,se existir
        string filename;
        input >> filename;
        image = loadFromPNG(filename); // carrega uma imagem lida dentro de um ficheiro PNG
    }
    void Script::blank() {
        clear_image_if_any(); // elimina a imagem atual,se existir
        int w, h;
        Color fill;
        input >> w >> h >> fill; // recebe como inputs o comprimento, a largura e cor com que a imagem deve ser criada.
        image = new Image(w, h, fill); // cria uma imagem com tamanho w e h com apenas uma cor em todos os p
    }
    void Script::save() {
        string filename;
        input >> filename;
        saveToPNG(filename, image);// guarda a imagem atual dentro de um ficheiro PNG
    }
    void Script::invert() {
        // cada pixel (r, g, b) é transformada em (255-r,255-g,255-b)
        int width = image->width(); // é retirada o comprimento da imagem atual
        int height = image->height(); // é retirada a altura da imagem atual
        for (int y = 0; y < height; y++) { // um ciclo for para andar de pixel em pixel em y
            for (int x = 0; x < width; x++) { //um ciclo for para andar de pixel em pixel em x
                Color& pixel = image -> at(x, y); // o pixel atual
                pixel.red() = 255 - pixel.red(); // inversão da cor vermelha do pixel;
                pixel.green() = 255 - pixel.green(); // inversão da cor verde do pixel;
                pixel.blue() = 255 - pixel.blue(); // inversão da cor azul do pixel;
            }
        }
    }
    void Script::toGrayScale() {
        // cada pixel (r, g, b) é transformada em (gray, gray, gray), onde gray = (r + g + b)/3
        int width = image->width(); // é retirada o comprimento da imagem atual
        int height = image->height(); // é retirada a altura da imagem atual
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color& pixel = image -> at(x, y); // o pixel atual
                int gray = (pixel.red() + pixel.green() + pixel.blue()) / 3; // é calculada uma cor comum a partir da média da cor vermelha ,azul e verde do pixel
                pixel.red() = gray; // trocada pela cor comum
                pixel.green() = gray; // trocada pela cor comum
                pixel.blue() = gray; // trocada pela cor comum
            }
        }
    }

    bool comparar(Color c1, Color c2) { //compara dois pixeis e verifica se têm igual valor , se tiverem retorna true
        if (c1.red() == c2.red() && c1.green() == c2.green() && c1.blue() == c2.blue()) {
            return true;
        }
        return false;
    }
    void Script::replace(){
        // Troca todos os pixeis de uma cor em especifico (r1, g1, b1) por outra cor (r2, g2, b2)
        Color c1;
        Color c2;
        input >> c1 >> c2; // recebe como input duas cores;
        int width = image->width();//é retirada o comprimento da imagem atual
        int height = image->height(); // é retirada a altura imagem atual
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color& pixel = image -> at(x, y); // o pixel atual
                if (comparar(pixel, c1)) { // se o pixel tiver a mesma cor que c1 troca por c2
                    pixel = c2;
                }
            }
        }
    }
    void Script::fill() {
        // Dentro de um quadrado de píxeis, todos os pixeis têm a sua cor alterada por uma única
        int x, y, w, h;
        Color c;

        input >> x >> y >> w >> h >> c; // recebe o x inicial, o y inicial, o x final, o y final e a cor desse quadrado

        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                int posX = x + i;
                int posY = y + j;

                Color& pixel = image-> at(posX, posY); // aplica a cor ao pixel atual
                pixel = c;
            }
        }
    }
    void Script::hMirror() {
        // A imagem é espelhada ao meio numa linha vertical imaginária ao centro
        int width = image->width(); //é retirada o comprimento da imagem atual
        int height = image->height(); // é retirada a altura imagem atual

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width / 2; x++) { // este for apenas irá até metade do comprimento
                Color& left_pixel = image -> at(x, y); //pixel da esquerda
                Color& right_pixel = image -> at(width - 1 - x, y); //pixel da direita
                swap(left_pixel.red(), right_pixel.red()); // troca da cor vermelha do pixel esquerda para o pixel direito
                swap(left_pixel.green(), right_pixel.green()); // troca da cor verde do pixel esquerda para o pixel direito
                swap(left_pixel.blue(), right_pixel.blue());  // troca da cor azul do pixel esquerda para o pixel direito
            }
        }
    }
    void Script::vMirror() {
        // A imagem é espelhada ao meio numa linha horizontal imaginária ao centro
        int width = image->width(); //é retirada o comprimento da imagem atual
        int height = image->height(); // é retirada a altura imagem atual

        for (int y = 0; y < height / 2; y++) {
            for (int x = 0; x < width; x++) {
                Color& top_pixel = image-> at(x, y); //pixel de cima
                Color& bottom_pixel = image -> at(x, height - 1 - y); //pixel de baixo
                swap(top_pixel.red(), bottom_pixel.red()); // troca da cor vermelha do pixel de cima para o pixel de baixo
                swap(top_pixel.green(), bottom_pixel.green());  // troca da cor verde do pixel de cima para o pixel de baixo
                swap(top_pixel.blue(), bottom_pixel.blue()); // troca da cor azul do pixel de cima para o pixel de baixo
            }
        }
    }
    void Script::add() {
        // copia todos os pixeis da imagem do ficheiro PNG dentro de um retangulo (x,y), exceto os pixeis que são iguais aos pixeisa da imagem atual na mesma posição
        string filename;
        int x, y;
        Color c;
        input >> filename >> c >> x >> y; // recebe o ficheiro de uma imagem PNG, a cor e o comprimento e a altura da imagem

        Image* addedImage = loadFromPNG(filename); // cria uma nova imagem a partir de um ficheiro PNG
        if (addedImage != nullptr && image != nullptr) {
            int addedWidth = addedImage->width(); // comprimento da imagem adicionada
            int addedHeight = addedImage->height(); //largura da imagem adicionada
            int imageWidth = image->width(); // comprimento da imagem
            int imageHeight = image->height(); // largura da imagem

            for (int j = 0; j < addedHeight; j++) {
                for (int i = 0; i < addedWidth; i++) {
                    Color& srcPixel = addedImage -> at(i, j); // pixel da imagem adicionada
                    int destX = x + i; // posição do pixel da imagem atual
                    int destY = y + j;  // posição do pixel da imagem atual
                    if (destX >= 0 && destX < imageWidth && destY >= 0 && destY < imageHeight) {
                        Color& destPixel = image -> at(destX, destY); //pixel da imagem atual
                        if (!comparar(srcPixel, c)) { // se não forem iguais, então o pixel da imagem atual será substituido
                            destPixel = srcPixel;
                        }
                    }
                }
            }

            delete addedImage; // deleta a imagem adicionada após as substituições dos píxeis
        }
    }
    void Script::crop() {
        int x, y, w, h;
        input >> x >> y >> w >> h;
        if (image != nullptr) {
            int imageWidth = image->width(); // comprimento da imagem
            int imageHeight = image->height(); // altura da imagem
            x = max(x, 0);
            y = max(y, 0);
            w = min(w, imageWidth - x);
            h = min(h, imageHeight - y);
            // define o retangulo da imagem q n irá ser cortada
            Image* croppedImage = new Image(w, h);
            for (int j = 0; j < h; j++) {
                for (int i = 0; i < w; i++) {
                    Color& srcPixel = image -> at(x + i, y + j); // pixel atual
                    Color& destPixel = (*croppedImage).at(i, j); // pixel da imagem cortada
                    destPixel = srcPixel; // troca o pixel da imagem atual dentro do retangulo que irá ser cortado para a nova imagem cortada criada
                }
            }
            delete image; //elimina a imagem pré-corte
            image = croppedImage; // a imagem atual passa a ser a iamagem com os cortes efetuados
        }
    }
    void Script::rotateRight() {
        // Roda a a imagem em 90º para a direita
        int width = image->width(); //comprimento da imagem
        int height = image->height(); // altura da imagem
        Image* rotatedImage = new Image(height, width); //comprimento da imagem rodada (troca-se o comprimento pela altura e vice-versa
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color& srcPixel = image -> at(x, y); // pixel atual
                Color& destPixel = rotatedImage -> at(height - 1 - y, x); //o pixel da imagem rotacionada mantem o msm x mas para girar 90º para a direita o y será subtraido da altura máxima
                destPixel = srcPixel;
            }
        }
        delete image; // elimina a imagem atual para salvar espaço e substitui pela imagem rodada
        image = rotatedImage;
    }
    void Script::rotateLeft() {
        // Roda a a imagem em 90º para a esquerda
        int width = image->width(); //comprimento da imagem
        int height = image->height(); // altura da imagem
        Image* rotatedImage = new Image(height, width); //comprimento da imagem rodada (troca-se o comprimento pela altura e vice-versa
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color& srcPixel = image -> at(x, y); // pixel atual
                Color& destPixel = rotatedImage -> at(y, width - 1 - x); //o pixel da imagem rotacionada mantem o msm y mas para girar 90º para a esquerda o y será subtraido do comprimento máximo
                destPixel = srcPixel;
            }
        }
        delete image; //elimina a imagem atual para salvar espaço e substitui pela imagem rodada
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

