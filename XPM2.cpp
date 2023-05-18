#include "XPM2.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

namespace prog {

    bool comparar1(Color c1, Color c2) {
        if (c1.red() == c2.red() && c1.green() == c2.green() && c1.blue() == c2.blue()) {
            return true;
        }
        return false;
    }



    Image* loadFromXPM2(const std::string& file) {
        ifstream in(file);
        vector<string> v,c; //string
        vector<char> s;
        string line;
        if (!in.is_open()) {
            cout << "Falha ao abrir o Ficheiro" << endl;
            return nullptr;
        } else {
            while (getline(in, line)) {
                v.push_back(line);
            }
        }
        in.close();
        istringstream iss(v[1]);
        int w, h, n, ch;            
        iss >> w >> h >> n >> ch;
        string codigo;
        char ignorar;
        char simbolo;
        for (int i = 0; i < n; i++) {
            istringstream iss(v[2 + i]);
            iss >> simbolo >> ignorar >> codigo;
            s.push_back(simbolo);                
            c.push_back(codigo);
        }
        for (int i = 0; i < int(c.size()); i++) {
            c[i] = c[i].substr(1);  
        }
            
        Image* xpm2Image = new Image(w, h);
        for (int y = 0; y < h; y++) {              
            for (int x = 0; x < w; x++) {
                for (int i = 0; i < n; i++) {                      
                     if (v[2 + n + y][x] == s[i]) {
                        istringstream ss(c[i]);
                        unsigned int cor;
                        ss >> hex >> cor;
                        rgb_value r = (cor >> 16) & 0xFF;
                        rgb_value g = (cor >> 8) & 0xFF;
                        rgb_value b = cor & 0xFF;
                        Color& pixel = xpm2Image->at(x, y);
                        pixel.red() = r;
                        pixel.green() = g;
                        pixel.blue() = b;
                        break;
                    }
                }
            }
        }
        return xpm2Image;
    }


    string hac(Color p) {
        
        stringstream streamr;
        streamr << std::hex << int(p.red());

        stringstream streamg;
        streamg << std::hex << int(p.green());

        stringstream streamb;
        streamb << std::hex << int(p.blue());

        string vermelho = streamr.str();
        string verde = streamg.str();
        string azul = streamb.str();

        if (vermelho.length() < 2) {
            vermelho = '0' + vermelho;
        }
        if (verde.length() < 2) {
            verde = '0' + verde;
        }
        if (azul.length() < 2) {
            azul = '0' + azul;
        }

        return vermelho + verde + azul;
    }


    bool verificar(Color c, vector<Color> v) {
        for (int i = 0; i < int(v.size()); i++) {
            if (comparar1(c, v[i])) {
                return true;
            }
        }
        return false;
    }

    void saveToXPM2(const std::string& file, const Image* img) {
        ofstream out(file);
        if (!out.is_open()) {
            cout << "Falha ao abrir o ficheiro para escrever" <<endl;
            return;
        }
        

        int width = img -> width();
        int height = img -> height();

        vector<char> key;
        vector<Color> cor;

        int j = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width;x++) {
                if (!verificar(img -> at(x, y), cor)) {
                    cor.push_back(img -> at(x, y));
                    key.push_back('!' + j);
                    j++;
                }
            }
        }
        int n_of_colors = int(cor.size());

        vector<string> hEX;

        for (int i = 0; i < n_of_colors; i++) {
            hEX.push_back(hac(cor[i])); 
        }
        
        out << "! XPM2" << endl;
        out << width << " " << height << " " << n_of_colors << " 1" << endl;
        for (int j = 0; j < n_of_colors; j++) {
            out << key[j] << " c #" << hEX[j] << endl; 
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                for (int n = 0; n < n_of_colors; n++) {
                    if (comparar1(img -> at(x, y), cor[n])) {
                        out << key[n];
                    }
                }
            }
            out << endl;
        }
        
    }

}
