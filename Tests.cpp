#include <'Estructura.hpp'>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <random>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace std;

//Función para generar números aleatorios
int random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

//Función para generar números aleatorios con distribución normal
int random_normal(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(min, max);
    return d(gen);
}

//Función para generar un par de rectángulos aleatorios
vector<Rectangle> random_rectangles(int min, int max) {
    vector<Rectangle> rects;
    for (int i = 0; i < 2; i++) {
        Rectangle rect;
        rect.x1 = random(min, max);
        rect.y1 = random(min, max);
        rect.x2 = random(rect.x1, max);
        rect.y2 = random(rect.y1, max);
        rects.push_back(rect);
    }
    return rects;
}
//Función para generar un par de rectángulos aleatorios con distribución normal
vector<Rectangle> random_rectangles_normal(int min, int max) {
    vector<Rectangle> rects;
    for (int i = 0; i < 2; i++) {
        Rectangle rect;
        rect.x1 = random_normal(min, max);
        rect.y1 = random_normal(min, max);
        rect.x2 = random_normal(rect.x1, max);
        rect.y2 = random_normal(rect.y1, max);
        rects.push_back(rect);
    }
    return rects;
}

//Main para generar y graficar los rectángulos aleatorios
int main() {
    //Generación de los rectángulos aleatorios
    vector<Rectangle> rects = random_rectangles(0, 100);
    //Impresión de los rectángulos aleatorios
    cout << "Rectángulos aleatorios: " << endl;
    for (int i = 0; i < rects.size(); i++) {
        cout << "Rectángulo " << i << ": " << endl;
        cout << "x1: " << rects[i].x1 << endl;
        cout << "y1: " << rects[i].y1 << endl;
        cout << "x2: " << rects[i].x2 << endl;
        cout << "y2: " << rects[i].y2 << endl;
    }
    //Generación de los rectángulos aleatorios con distribución normal
    vector<Rectangle> rects_normal = random_rectangles_normal(0, 100);
    //Impresión de los rectángulos aleatorios con distribución normal
    cout << "Rectángulos aleatorios con distribución normal: " << endl;
    for (int i = 0; i < rects_normal.size(); i++) {
        cout << "Rectángulo " << i << ": " << endl;
        cout << "x1: " << rects_normal[i].x1 << endl;
        cout << "y1: " << rects_normal[i].y1 << endl;
        cout << "x2: " << rects_normal[i].x2 << endl;
        cout << "y2: " << rects_normal[i].y2 << endl;
    }
    //Generación de los rectángulos aleatorios con distribución normal
    vector<Rectangle> rects_normal2 = random_rectangles_normal(0, 100);
    //Impresión de los rectángulos aleatorios con distribución normal
    cout << "Rectángulos aleatorios con distribución normal: " << endl;
    for (int i = 0; i < rects_normal2.size(); i++) {
        cout << "Rectángulo " << i << ": " << endl;
        cout << "x1: " << rects_normal2[i].x1 << endl;
        cout << "y1: " << rects_normal2[i].y1 << endl;
        cout << "x2: " << rects_normal2[i].x2 << endl;
        cout << "y2: " << rects_normal2[i].y2 << endl;
    }
    return 0;
}