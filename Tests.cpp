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
    //Valores aleatorios entre 0 y 500mil
    uniform_int_distribution<> distr1(0, 500000);
    //El tamaño de los lados de los rectángulos es aleatorio entre 0 y 100
    uniform_int_distribution<> distr2(0, 100);

    int j = pow(2,n); // Número de rectángulos
    for (int i=0; i<n; i++){
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
    }
    
}

void test1(){

}
void test2(){
    ofstream archivo("resultados.txt");
    if (archivo.is_open()){
        //Encabezado del archivo
        archivo << "Algoritmo 1, Potencia de N, Tiempo de ejecución" << endl;
        //Cuerpo del archivo (se testea desde n=2^10 hasta 2^25)
        // for (int i=10; i<=25; i++){
        //     int n = pow(2,i);
        //     vector<unsigned int> input(n,0);
        //     for (int j=0; j<n; j++){
        //         input[j] = j+1;
        //     }
        //     auto start = chrono::steady_clock::now();
        //     makeGroups(input, 2);
        //     auto end = chrono::steady_clock::now();
        //     auto diff = end - start;
        //     archivo << "Algoritmo 1, " << n << ", " << chrono::duration <double, milli> (diff).count() << endl;
        // }






    }
    archivo.close();

}   

int main(){
    test1();
    test2();
    return 0;
}
