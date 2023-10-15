#include <vector>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <random>
#include <math.h>
#include "algoritmo1.hpp"
using namespace std;

// Se crea función que recibe un vector de rectángulos y los ordena según la coordenada X del centro del rectángulo
void ordenarRectangulosX(vector<Rectangle> &rects){
    sort(rects.begin(), rects.end(), [](const Rectangle &a, const Rectangle &b){
        float mean_a = (a.x1 + a.x2) / 2.0f;
        float mean_b = (b.x1 + b.x2) / 2.0f;
        return mean_a < mean_b; });
}
// Se crea función que convierte un punto a su representación en la curva de Hilbert
//convierte (x,y) a d
//rotar/voltear un cuadrante apropiadamente
void rot(int n, int *x, int *y, int rx, int ry) {
    int t;
    if (ry == 0) {
        if (rx == 1) {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }
        t  = *x;
        *x = *y;
        *y = t;
    }
}
int xy2d (int n, int x, int y) {
    int rx, ry, s, d=0;
    for (s = n/2; s > 0; s /= 2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    }
    return d;
}

//convierte d a (x,y)
void d2xy(int n, int d, int *x, int *y) {
    int rx, ry, s, t=d;
    *x = *y = 0;
    for (s = 1; s < n; s *= 2) {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        rot(s, x, y, rx, ry);
        *x += s * rx;
        *y += s * ry;
        t /= 4;
    }
}

// Se crea función que ordena los centros de los rectángulos en función
// de su valor dentro de la curva de Hilbert
void ordenarRectangulosHilbert(vector<Rectangle> &rects){
    sort(rects.begin(), rects.end(), [](const Rectangle &a, const Rectangle &b){
        int n = 2;
        float x1 = (a.x2-a.x1)/2.0f;
        float y1 = (a.y2-a.y1)/2.0f;
        float x2 = (b.x2-b.x1)/2.0f;
        float y2 = (b.y2-b.y1)/2.0f;

        int d1 = xy2d(n, x1, y1);
        int d2 = xy2d(n, x2, y2);
        return d1 < d2; });
}

void testHilbert(){
    //Crea 6 rectángulos aleatorios en un cuadrante de 20x20
    vector<Rectangle> rects;
    random_device rd;
    mt19937 gen(rd());
    //Valores aleatorios entre 0 y 20
    uniform_int_distribution<> distr1(0, 20);
    //El tamaño de los lados de los rectángulos es aleatorio entre 0 y 10
    uniform_int_distribution<> distr2(0, 10);

    for (int i = 0; i < 6; i++){
        int x1 = distr1(gen);
        int y1 = distr1(gen);
        int x2 = x1 + distr2(gen);
        int y2 = y1 + distr2(gen);
        Rectangle rect;
        rect.x1 = x1;
        rect.y1 = y1;
        rect.x2 = x2;
        rect.y2 = y2;
        cout << "Rectangulo " << i + 1 << ": " << x1 << ", " << x2 << " " << y1 << ", " << y2 << endl;
        rects.push_back(rect);
    }
    //Se ordenan los rectángulos según su centro en la curva de Hilbert
    ordenarRectangulosHilbert(rects);
    for (int i = 0; i < rects.size(); i++){
        cout << "Rectangulo " << i + 1 << ": " << rects[i].x1 << ", " << rects[i].x2 << " " << rects[i].y1 << ", " << rects[i].y2 << endl;
    }

}



Node *makeLeaf(Rectangle rect){
    Node *node = new Node;
    node->MBR = rect;
    node->isLeaf = true;
    return node;
}
// Función que agrupa M nodos en un nodo padre
Node *makeParent(vector<Node *> children){
    Node *node = new Node;
    node->isLeaf = false;
    node->children = children;

    // Se calcula el rectángulo que contiene a todos los hijos
    int x_chiquito = children[0]->MBR.x1; // pivote
    int x_grande = children[0]->MBR.x2;
    int y_chiquito = children[0]->MBR.y1;
    int y_grande = children[0]->MBR.y2;
    for (int i = 0; i < children.size(); i++){

        if (children[i]->MBR.x1 < x_chiquito)
            x_chiquito = children[i]->MBR.x1;

        if (children[i]->MBR.x2 > x_grande)
            x_grande = children[i]->MBR.x2;

        if (children[i]->MBR.y1 < y_chiquito)
            y_chiquito = children[i]->MBR.y1;

        if (children[i]->MBR.y2 > y_grande)
            y_grande = children[i]->MBR.y2;
    }

    Rectangle rect = {x_chiquito, y_chiquito, x_grande, y_grande};
    node->MBR = rect;
    return node;
}

// Función que crea hojas a partir de un vector de rectángulos que primero ordena
vector<Node *> makeLeaves(vector<Rectangle> rects){
    vector<Node *> leaves;
    ordenarRectangulosX(rects);
    for (int i = 0; i < rects.size(); i++){
        Node *node = makeLeaf(rects[i]);
        leaves.push_back(node);
    }
    return leaves;
}
// Función que recibe un vector de nodos y hace grupos
RTree makeGroups(vector<Node *> children, int M){
    vector<Node *> grupo; //
    if (children.size() == 1){
        RTree rtree = RTree();
        rtree.hijos = children;
        rtree.root = children[0];
        return rtree;
    }
    for (int i = 0; i < (int)ceil((double)children.size() / M); i++){
        vector<Node *> grupito;
        for (int j = 0; j < M; j++){
            int index = i * M + j;
            if (index >= children.size()){
                break;
            }
            grupito.push_back(children[index]);
        }
        Node *node = makeParent(grupito);

        grupo.push_back(node);
        cout << "Nodo grupito: " << node->MBR.x1 << ", " << node->MBR.x2 << endl;
        for (int k = 0; k < grupito.size(); k++){
            cout << "Nodo hoja: " << grupito[k]->MBR.x1 << ", " << grupito[k]->MBR.x2 << endl;
        }
        grupito.clear();
    }
    return makeGroups(grupo, M);
}



void test(){

}
int main(){
    //test();
    testHilbert();
    return 0;
}
