#include <vector>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <random>
#include <math.h>
#include <cmath>
#include "algoritmo1.hpp" // Include the header file with the correct path
#include <vector>

#include <iostream>
#include <ostream>
#include <algorithm>
#include <random>
#include <math.h>
#include <cmath>
using namespace std;

// Tenemos n puntos en el plano (rectangulo tal que x1=x2 e y1=y2)
// y queremos encontrar el rectángulo de área mínima que los contiene a todos.

/* Paso 1: ordenar los puntos según la coordenada X
 * Podemos reutilizar la funcion ordenarRectangulosX sin necesidad de calcular el promedio
 * de las coordenadas X de cada rectangulo, ya que en este caso x1=x2 */
void ordenarRectangulosX(vector<Rectangle> &rects){
    sort(rects.begin(), rects.end(), [](const Rectangle &a, const Rectangle &b){
        float mean_a = (a.x1 + a.x2) / 2.0f;
        float mean_b = (b.x1 + b.x2) / 2.0f;
        return mean_a < mean_b; });
}
void ordenarRectangulosY(vector<Rectangle> &rects){
    sort(rects.begin(), rects.end(), [](const Rectangle &a, const Rectangle &b){
        float mean_a = (a.y1 + a.y2) / 2.0f;
        float mean_b = (b.y1 + b.y2) / 2.0f;
        return mean_a < mean_b; });
}

/* Paso 2: Dividir el resultado en sqrt(n/M) grupos*/

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
        for (int j = 0; j < M; j++){
            int index = i * M + j;
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

// Función que recibe un vector vectores de puntos y hace grupos de tamaño sqrt(n/M)
// Estos si son nodos
vector<Node *> makeYGroups(vector<vector<Rectangle>> XGroups, int M){
    vector<Node *> grupo; // este grupo es vector de nodos
    for (int i = 0; i < XGroups.size(); i++){ // hay que crear un rectangulo que contenga los puntos
        vector<Node *> points_leaves = makeLeavesY(XGroups[i]); // convertir puntos en hojas
        vector<Node *> grupito; // grupito es vector de nodos
        for (int j = 0; j < (int)ceil((double)XGroups[i].size()/M); j++){
            grupito.push_back(points_leaves[j]);
        }
        Node *node = makeParent(grupito);
        grupo.push_back(node);
        grupito.clear();
    }
    return grupo; // vector con todos los MBR que contienen a los puntos (nodos)
}

// Función Sort-Tile-Recursive que construye un R-tree
RTree SortTileRecursive(vector<Rectangle> points, int M){
    // Se ordenan los puntos segun su coordenada X
    ordenarRectangulosX(points);
    // Se hacen sqrt(n/M) grupos
    vector<vector<Rectangle>> XGroups = makeXGroups(points, M);
    // Ordenar los puntos de cada grupo según su coordenada Y
    for (int i = 0; i < XGroups.size(); i++){
        ordenarRectangulosY(XGroups[i]); // Ordena la coordenada Y en cada grupito
    }
    // Se hacen M grupos dentro de cada XGroup
    vector<Node *> leaves = makeYGroups(XGroups, M); // Recién en este punto se tienen hojas
    // Recursión del árbol (caso base)
    if (leaves.size() == 1){
        RTree rtree = RTree();
        rtree.hijos = leaves;
        rtree.root = leaves[0];
        return rtree;
    }
    vector<Rectangle> leaf_points = makePoints(leaves); // Transformamos las hojas a puntos para hacer recursión
    // Hasta aquí se tienen los nodos hoja sobre los que hay que hacer la recursión
    return SortTileRecursive(leaf_points,M);
}

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
    for (int i=0; i<points.size(); i++) {
        cout << "Coordenada X: " << points[i].x1 << endl;
    }
    // Agruparlos en sqrt(n/M) grupos según su coordenada X
    vector<vector<Rectangle>> XGroups = makeXGroups(points, 6);

    /*
    cout << "Resultado: " << endl;
    for (int i=0; i<points.size(); i++) {
        cout << "Rectangulo " << i+1 << ": " << points[i].x1 << ", " << result[i].x2 << " " << result[i].y1 << ", " << result[i].y2 << endl;
    }*/
    return 0;
}