#include <vector>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <random>
#include <math.h>
#include <cmath>
#include "Structures.hpp"
#include <vector>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <random>
#include <math.h>
#include <cmath>
using namespace std;

/* Tenemos n puntos en el plano (rectangulo tal que x1=x2 e y1=y2)
 * y queremos encontrar el rectángulo de área mínima que los contiene a todos. */

// Ordena rectangulos/puntos segun su coordenada X
void ordenarRectangulosX(vector<Rectangle> &rects){
    sort(rects.begin(), rects.end(), [](const Rectangle &a, const Rectangle &b){
        float mean_a = (a.x1 + a.x2) / 2.0f;
        float mean_b = (b.x1 + b.x2) / 2.0f;
        return mean_a < mean_b; });
}

// Ordena rectangulos/puntos segun su coordenada Y
void ordenarRectangulosY(vector<Rectangle> &rects){
    sort(rects.begin(), rects.end(), [](const Rectangle &a, const Rectangle &b){
        float mean_a = (a.y1 + a.y2) / 2.0f;
        float mean_b = (b.y1 + b.y2) / 2.0f;
        return mean_a < mean_b; });
}

// Función que recibe un rectángulo y lo hace un nodo
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

// Función que crea hojas a partir de un vector de rectángulos que primero ordena la coordenada Y
vector<Node *> makeLeavesX(vector<Rectangle> rects){
    vector<Node *> leaves;
    ordenarRectangulosX(rects);
    for (int i = 0; i < rects.size(); i++){
        Node *node = makeLeaf(rects[i]);
        leaves.push_back(node);
    }
    return leaves;
}

// Función que crea hojas a partir de un vector de rectángulos que primero ordena la coordenada Y
vector<Node *> makeLeavesY(vector<Rectangle> rects){
    vector<Node *> leaves;
    ordenarRectangulosY(rects);
    for (int i = 0; i < rects.size(); i++){
        Node *node = makeLeaf(rects[i]);
        leaves.push_back(node);
    }
    return leaves;
}

// Función que convierte un vector de nodos en un vector de puntos tomando su MBR
vector<Rectangle> makePoints(vector<Node *> leaves){
    vector<Rectangle> rectangles;
    for (int i = 0; i < leaves.size(); i++){
        Rectangle rectangle = leaves[i]->MBR;
        rectangles.push_back(rectangle);
    }
    return rectangles;
}

// Función que recibe un vector de puntos y hace grupos de tamaño sqrt(n/M)
vector<vector<Rectangle>> makeXGroups(vector<Rectangle> points, int M){
    vector<vector<Rectangle>> grupo; // grupo es vector de vectores de puntos
    for (int i = 0; i < (int)ceil(sqrt((double)points.size() / M)); i++){
        vector<Rectangle> grupito; // grupito es vector de puntos
        for (int j = 0; j < (int)ceil(sqrt((double)points.size() * M)); j++){
            int index = i * (int)ceil(sqrt((double)points.size() * M)) + j;
            if (index >= points.size()){
                break;
            }
            grupito.push_back(points[index]);
        }
        grupo.push_back(grupito);
        grupito.clear();
    }
    return grupo;
}

// Función que recibe un vector vectores de puntos y hace sqrt(n/M) grupos de tamaño M
vector<Node *> makeYGroups(vector<vector<Rectangle>> XGroups, int M, int factor, int nivel){
    string nombre = "groupsSTR" + to_string(factor) +"Nivel" + to_string(nivel) + ".bin";
    vector<Node *> grupo; // este grupo es vector de nodos
    for (int i = 0; i < XGroups.size(); i++){ // hay que crear un rectangulo que contenga los puntos
        vector<Node *> points_leaves = makeLeavesY(XGroups[i]); // convertir puntos en hojas
        vector<Node *> grupito; // grupito es vector de nodos
        for (int j = 0; j < (int)ceil((double)XGroups[i].size()/M); j++) { // sqrt(n/M) nodos
            for(int k = 0; k < M; k++){
                int index = j * M + k;
                if (index >= XGroups[i].size()){
                    break;
                }
                grupito.push_back(points_leaves[index]);
            }
            Node *node = makeParent(grupito);
            grupo.push_back(node);
            grupito.clear();
        }
    }
    FILE *arch = fopen(nombre.c_str(), "wb");
    if (arch == NULL){
        std::cout << "Error al abrir el archivo" << std::endl;
        return;
    }
    for (int i = 0; i < grupo.size(); i++){
        fwrite(grupo[i], sizeof(Node), 1, arch);
    }
    return grupo; // vector con todos los MBR que contienen a los puntos (nodos)
}

// Función Sort-Tile-Recursive que construye un R-tree
RTree SortTileRecursive(vector<Rectangle> points, int M, int factor) {
    // Convertimos los puntos iniciales en hojas
    vector<Node *> leaves = makeLeaves(points);
    // Creamos los archivos con las hojas
    string nombre = "leavesSTR" + to_string(factor) + ".bin";
    FILE *arch = fopen(nombre.c_str(), "wb");
    if (arch == NULL){
        std::cout << "Error al abrir el archivo" << std::endl;
        return RTree();
    }
    for (int i = 0; i < leaves.size(); i++){
        fwrite(leaves[i], sizeof(Node), 1, arch);
    }
    int nivel = 0;
    int result = ceil((log10(pow(2,  nivel + 10)) / log10(1024)));
    // Se ordenan los puntos segun su coordenada X
    ordenarRectangulosX(points);
    // Se hacen sqrt(n/M) grupos
    vector<vector<Rectangle>> XGroups = makeXGroups(points, M);
    // Ordenar los puntos de cada grupo según su coordenada Y
    for (int i = 0; i < XGroups.size(); i++){
        ordenarRectangulosY(XGroups[i]); // Ordena la coordenada Y en cada grupito
    }
    // Se hacen M grupos dentro de cada XGroup
    leaves = makeYGroups(XGroups, M, factor, result); // Recién en este punto se tienen hojas
    fclose(arch);
    // Recursión del árbol (caso base)
    if (leaves.size() == 1){
        RTree rtree = RTree();
        rtree.hijos = leaves;
        rtree.root = leaves[0];
        return rtree;
    }
    vector<Rectangle> leaf_points = makePoints(leaves); // Transformamos las hojas a puntos para hacer recursión
    // Se llama recursivamente a la función
    return SortTileRecursive(leaf_points, M, factor);
}

/*
//Main que genera aleatoriamente 6 rectángulos en un plano de 20x20 y los agrupa en un R-tree
//utiliza include random
int main() {
    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<> distr(0, 20);
    //Se crea un vector de rectángulos
    vector<Rectangle> points;
    //Se crean 6 puntos aleatorios
    for (int i = 0; i < 60; i++) {
        int x = distr(eng);
        int y = distr(eng);
        Rectangle rect = {x, y, x, y};
        points.push_back(rect);
        cout << "Rectangulo " << i+1 << ": " << x << ", " << y << endl;

    }
    // Se ordena la coordenada x de los puntos 
    ordenarRectangulosX(points);
    // Agruparlos en sqrt(n/M) grupos según su coordenada X
    vector<vector<Rectangle>> XGroups = makeXGroups(points, 4);
    for (int k = 0; k < XGroups.size(); k++){
        cout << "Grupo " << k+1 << ": "<< endl;
        for(int i = 0; i < XGroups[k].size(); i++) {
            cout << "Punto " << i+1 << ": " << XGroups[k][i].x1 << ", " << XGroups[k][i].y1 << endl;
        }
    }
    // Ordenar los puntos de cada grupo según su coordenada Y
    for (int i = 0; i < XGroups.size(); i++){
        ordenarRectangulosY(XGroups[i]); // Ordena la coordenada Y en cada grupito
    }
    // Se hacen sqrt(n/M) grupos de tamaño M dentro de cada XGroup
    vector<Node *> leaves = makeYGroups(XGroups, 4); // Recién en este punto se tienen n/M nodos
    for (int k = 0; k < leaves.size(); k++){
        cout << "Hoja " << k+1 << ": (" << leaves[k]->MBR.x1 << ", " << leaves[k]->MBR.y1 << ") (" 
        << leaves[k]->MBR.x2 << ", " << leaves[k]->MBR.y2 << ")" << endl;
    }
    // Se repite recursivamente
    RTree rtree = SortTileRecursive(makePoints(leaves), 4);
    cout << "Nodo raiz: (" << rtree.root->MBR.x1 << ", " << rtree.root->MBR.y1 << ") (" 
        << rtree.root->MBR.x2 << ", " << rtree.root->MBR.y2 << ")" << endl;
    return 0;
}
*/