#include "algoritmo1.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include<algorithm>
#include<random>
#include<chrono>
#include <fstream>
using namespace std;

string print(vector<unsigned int> arr){
    string res;
    for (unsigned int i=0;i<arr.size();i++){
            res+= to_string(arr[i])+" ";
        }

    return res;
}
vector<Rectangle> createRandomRectangles(int n){
    vector<Rectangle> rects;
    random_device rd;
    mt19937 gen(rd());
    // //Valores aleatorios entre 0 y 500mil
    // uniform_int_distribution<> distr1(0, 500000);
    // //El tamaño de los lados de los rectángulos es aleatorio entre 0 y 100
    // uniform_int_distribution<> distr2(0, 100);

    //Valores aleatorios entre 0 y 20
    uniform_int_distribution<> distr1(0, 20);
    //El tamaño de los lados de los rectángulos es aleatorio entre 0 y 10
    uniform_int_distribution<> distr2(0, 10);

    int j = pow(2,n); // Número de rectángulos
    for (int i=0; i<j; i++){
        int x1 = distr1(gen);
        int y1 = distr1(gen);
        int x2 = x1 + distr2(gen);
        int y2 = y1 + distr2(gen);
        Rectangle rect;
        rect.x1 = x1;
        rect.y1 = y1;
        rect.x2 = x2;
        rect.y2 = y2;

        rects.push_back(rect);

        //Imprime los rectángulos generados
        cout << "Rectangulo " << i + 1 << ": " << x1 << ", " << x2 << " " << y1 << ", " << y2 << endl;
    }
    return rects;
    
}

void test1(){

}
void test2(){
    ofstream archivo("resultados.bin");
    if (archivo.is_open()){
        //Encabezado del archivo
        archivo << "Algoritmo 1, Potencia de N, Tiempo de ejecución" << endl;
        //Cuerpo del archivo (se testea desde n=2^10 hasta 2^25)
        //TODO: Se generan los rectángulos




    }
    archivo.close();

}   
void test3(){
    //Se generan los rectángulos
    vector<Rectangle> rects = createRandomRectangles(3);
    //Se construye el R-tree con el método de agrupamiento
    RTree rtree = nearestX(rects, 3);

    // //Se crea un rectángulo de búsqueda
    // Rectangle rect;
    // rect.x1 = 0;
    // rect.y1 = 0;
    // rect.x2 = 10;
    // rect.y2 = 10;
    // //Se busca en el R-tree los rectángulos que intersectan con el rectángulo de búsqueda
    // vector<Rectangle> result = rtree.search(rect);
    
    // Se serializa el R-tree para guardarlo en un archivo binario
    serialize(rtree, "rtree.bin");

}

int main(){
    // test1();
    // test2();
    test3();
    return 0;
}
