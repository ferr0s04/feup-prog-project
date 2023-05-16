#include "XPM2.hpp"
#include "Image.hpp"
#include "Color.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

namespace prog {
    Image* loadFromXPM2(const std::string& file) {
        ifstream in(file);
        vector<string> v,s;
        vector<int> c;
        string line;
        if(in.is_open()!=true){
            cout << "Falha ao abrir o Ficheiro" <<endl;
        }
        else {
            while(getline(in,line)){
                v.push_back(line);
            }
            in.close();
            istringstream iss(v[1]);
            int w,h,n,c;
            iss >> w >> h >> n >> c;
            for (int i=0;i<n;i++){
                string simbolo, lixo, codigo;
                istringstream iss(v[1+i]);
                iss >> simbolo >> lixo >> codigo;
                s.pushback(simbolo);
                c.pushback(codigo);
            }
            Image* Xmp2Image= new Image(w,h);
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    for (int i=0;i<n;i++){
                        if (v[1+n+y][x] == s[i]){
                            istringstream ss(c[i]);
                            unsigned int cor;
                            ss >> cor;
                           r = (cor >> 16) & 0xFF;
                           g=(cor >> 8) & 0xFF;
                           b=cor & 0xFF;
                            Color& pixel = image -> at(x,y);
                            pixel.red() = r;
                            pixel.green() = g;
                            pixel.blue() = b;


                        }


                    }


                }
            }
        }


    }


        return nullptr;
    }

    void saveToXPM2(const std::string& file, const Image* image) {

    }
}
