#include <vector>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <random>
#include <math.h>
#include "algoritmo1.hpp"
using namespace std;

// Se crea estructura para los rectángulos
struct Rectangle{
    // Coordenadas de la esquina inferior izquierda
    int x1, y1;
    // Coordenadas de la esquina superior derecha
    int x2, y2;

    // Se crea método para calcular el área de un rectángulo
    int area(){
        return abs(x2 - x1) * abs(y2 - y1);
    }
    int distX(){
        return abs(x2 - x1);
    }
    int distY(){
        return abs(y2 - y1);
    }
    float centerX(){
        return (x1 + x2) / 2.0f;
    }
};

// Se crea función que recibe un vector de rectángulos y los ordena según la coordenada X del centro del rectángulo
void ordenarRectangulosX(vector<Rectangle> &rects){
    sort(rects.begin(), rects.end(), [](const Rectangle &a, const Rectangle &b){
        float mean_a = (a.x1 + a.x2) / 2.0f;
        float mean_b = (b.x1 + b.x2) / 2.0f;
        return mean_a < mean_b; });
}

// Definimos los nodos, que son los rectángulos que se encuentran en el árbol
// Contienen un vector de nodos hijos y un vector de rectángulos
struct Node{
    bool isLeaf;
    Rectangle MBR;
    vector<Node *> children;
};
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
// Estructura R-tree
struct RTree{
    Node *root;
    int M;
    vector<Node *> hijos;
    // Búsqueda: retorna un vector de rectangulos que intersectan con value
    std::vector<Rectangle> search(const Rectangle& value) {
        std::vector<Rectangle> result;
        searchRecursive(root, value, result);
        return result;
    }
};
bool intersects(const Rectangle& rect1, const Rectangle& rect2) {
    // Verifica si dos rectángulos se intersectan.
    bool x_overlap = (rect1.x1 <= rect2.x2 && rect1.x2 >= rect2.x1) || (rect2.x1 <= rect1.x2 && rect2.x2 >= rect1.x1);
    bool y_overlap = (rect1.y1 <= rect2.y2 && rect1.y2 >= rect2.y1) || (rect2.y1 <= rect1.y2 && rect2.y2 >= rect1.y1);

    return x_overlap && y_overlap;
}
void searchRecursive(Node* currentNode, const Rectangle& value, std::vector<Rectangle>& result) {
    if(currentNode == nullptr)
        return;
    if(currentNode->isLeaf) {
        if (intersects(currentNode->MBR, value)) {
            result.push_back(currentNode->MBR);
        }
    } else {
        for (Node* child : currentNode->children) {
            if(intersects(child->MBR, value)) {
                searchRecursive(child, value, result);
            }
        }
    }
}

/*bool doRectanglesIntersect(const Rectangle& rect1, const Rectangle& rect2) {
    // Verifica si los rectángulos no se superponen
    if (rect1.x1 > rect2.x2 || // rect1 está a la derecha de rect2
        rect2.x1 > rect1.x2 || // rect1 está a la izquierda de rect2
        rect1.y1 > rect2.y2 || // rect1 está arriba de rect2
        rect2.y1 > rect1.y2) { // rect1 está abajo de rect2
        return false;
    }
    // Si no se cumple la condición anterior, los rectángulos se superponen
    return true;
}*/

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

// Función Nearest-X que construye un R-tree a partir de un vector de rectángulos y un parámetro M
RTree nearestX(vector<Rectangle> rects, int M){
    // Se crean las hojas
    vector<Node *> leaves = makeLeaves(rects);
    // Se crea el R-tree
    RTree rtree = makeGroups(leaves, M);
    return rtree;
}

void test(){
    int e = 10; //exponente
    while (e <= 25){   // genera aleatoriamente n rectángulos en un plano de 500mil x 500mil y los agrupa en un R-tree
        random_device rd;
        mt19937 eng(rd());
        uniform_int_distribution<> distr1(0, 500000);
        uniform_int_distribution<> distr2(0, 100);
        // Se crea un vector de rectángulos
        vector<Rectangle> rects;
        // Se crean 6 rectángulos aleatorios
        int n = pow(2,e); // Número de rectángulos
        int M = 3; // Máximo número de hijos por nodo
        for (int i = 0; i < n; i++){
            int a = distr1(eng);
            int b = distr2(eng);
            int c = distr1(eng);
            int d = distr2(eng);
            int x1 = a;
            int x2 = a + b;
            if (a + b > 500000){
                int x2 = 500000;
            }
            int y1 = c;
            int y2 = c + d;
            if (c + d > 500000){
                int y2 = 500000;
            }
            Rectangle rect = {x1, y1, x2, y2};
            rects.push_back(rect);
            cout << "Rectangulo " << i + 1 << ": " << x1 << ", " << x2 << " " << y1 << ", " << y2 << endl;
        }
        // Se hacen las hojas
        vector<Node *> leaves = makeLeaves(rects);
        for (int i = 0; i < n; i++){
            cout << "Centro: " << leaves[i]->MBR.centerX() << endl;
        }
        // Se crea el R-tree
        RTree rtree = makeGroups(leaves, M);
        // Impresión de los nodos hoja
        cout << "Nodos hoja: " << endl;

        cout << "Nodo " << ": " << rtree.root->MBR.x1 << ", " << rtree.root->MBR.x2 << endl;
        e++;
    }
}

void test2(){
    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<> distr1(0, 20);
    uniform_int_distribution<> distr2(0, 10);

    vector<Rectangle> rects;

    for (int i = 0; i < 10; i++){
            int a = distr1(eng);
            int b = distr2(eng);
            int c = distr1(eng);
            int d = distr2(eng);
            int x1 = a;
            int x2 = a + b;
            if (a + b > 30){
                int x2 = 30;
            }
            int y1 = c;
            int y2 = c + d;
            if (c + d > 30){
                int y2 = 30;
            }
            Rectangle rect = {x1, y1, x2, y2};
            rects.push_back(rect);
            cout << "Rectangulo " << i + 1 << ": " << x1 << ", " << x2 << " " << y1 << ", " << y2 << endl;

            vector<Node *> leaves = makeLeaves(rects);
            for (int i = 0; i < 10; i++){
                cout << "Centro: " << leaves[i]->MBR.centerX() << endl;
            }
            // Se crea el R-tree
            RTree rtree = makeGroups(leaves, 3);
            // Impresión de los nodos hoja
            cout << "Nodos hoja: " << endl;

            cout << "Nodo " << ": " << rtree.root->MBR.x1 << ", " << rtree.root->MBR.x2 << endl;
    }
}
int main(){
    test2();
    return 0;
}
