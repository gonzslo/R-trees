#include <vector>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <random>
using namespace std;

//Se crea estructura para los rectángulos
struct Rectangle {
    // Coordenadas de la esquina inferior izquierda
    int x1, y1;
    // Coordenadas de la esquina superior derecha
    int x2, y2;

    //Se crea método para calcular el área de un rectángulo
    int area() {
        return abs(x2 - x1) * abs(y2 - y1);
    }
    int distX(){
        return abs(x2 - x1);
    }
    int distY(){
        return abs(y2 - y1);
    }
};

// //main para probar rectangulos
// int main(){
//     Rectangle rect;
//     rect.x1 = 1;
//     rect.y1 = 1;
//     rect.x2 = 2;
//     rect.y2 = 2;
//     cout << rect.area() << endl;
//     //Imprime cada punto, las distancias y su area
//     cout << "Rectángulo: " << endl;
//     cout << "Punto 1: " << endl;
//     cout << rect.x1 ;
//     cout << ", " ;
//     cout << rect.y1 << endl;
//     //Segundo punto
//     cout << "Punto 2: " << endl;
//     cout << rect.x2;
//     cout << ", ";
//     cout << rect.y1 << endl;
//     //Tercer punto
//     cout << "Punto 3: " << endl;
//     cout << rect.x2;
//     cout << ", ";
//     cout << rect.y2 << endl;
//     //Cuarto punto
//     cout << "Punto 4: " << endl;
//     cout << rect.x1;
//     cout << ", ";
//     cout << rect.y2 << endl;
//     //Distancias
//     cout << "Distancia en x: " << endl;
//     cout << rect.x2 - rect.x1 << endl;
//     cout << "Distancia en y: " << endl;
//     cout << rect.y2 - rect.y1 << endl;
//     //Area
//     cout << "Area: " << endl;
//     cout << rect.area() << endl;
//     return 0;
// }

//Se guardan en vectores 10 rectángulos generados aleatoriamente
//El tamaño de cada lado de los rectángulos está entre 0 y 100
//Se crea un vector de struct Rectangle    
vector<Rectangle> generarRectangulos(){
    vector<Rectangle> rectangulos;
    //Se crea un generador de números aleatorios enteros
    random_device rd;
    //Se crea un motor de números aleatorios
    mt19937 gen(rd());
    //Se crea una distribución uniforme de números enteros entre 0 y 100
    uniform_int_distribution<> dis(0, 100);
    //Se generan 10 rectángulos
    for (int i = 0; i < 10; i++){
        //Se crea un rectángulo
        Rectangle rect;
        //Se generan las coordenadas de la esquina inferior izquierda
        rect.x1 = dis(gen);
        rect.y1 = dis(gen);
        //Se generan las coordenadas de la esquina superior derecha
        rect.x2 = dis(gen);
        rect.y2 = dis(gen);
        //Se agrega el rectángulo al vector de rectángulos
        rectangulos.push_back(rect);
    }
    return rectangulos;
}

int main(){
    vector<Rectangle> rectangulos = generarRectangulos();
    //Se imprimen los rectángulos generados
    cout << "Rectángulos generados: " << endl;
    for (int i = 0; i < rectangulos.size(); i++){
        cout << "Rectángulo " << i << ": " << endl;
        cout << "Punto 1: " << endl;
        cout << rectangulos[i].x1;
        cout << ", ";
        cout << rectangulos[i].y1 << endl;
        //Segundo punto
        cout << "Punto 2: " << endl;
        cout << rectangulos[i].x2;
        cout << ", ";
        cout << rectangulos[i].y1 << endl;
        //Tercer punto
        cout << "Punto 3: " << endl;
        cout << rectangulos[i].x2;
        cout << ", ";
        cout << rectangulos[i].y2 << endl;
        //Cuarto punto
        cout << "Punto 4: " << endl;
        cout << rectangulos[i].x1;
        cout << ", ";
        cout << rectangulos[i].y2 << endl;
        //Distancias
        cout << "Distancia en x: " << endl;
        cout << rectangulos[i].distX() << endl;
        cout << "Distancia en y: " << endl;
        cout << rectangulos[i].distY() << endl;
        //Area
        cout << "Area: " << endl;
        cout << rectangulos[i].area() << endl;
    }
    return 0;
}

