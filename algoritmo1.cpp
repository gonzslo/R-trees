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

//Se crea función que recibe un vector de rectángulos y los ordena según la coordenada X del centro del rectángulo
void ordenarRectangulosX(vector<Rectangle> &rects){
    sort(rects.begin(), rects.end(), [](const Rectangle& a, const Rectangle& b) {
        float mean_a = (a.x1 + a.x2) / 2.0f;
        float mean_b = (b.x1 + b.x2) / 2.0f;
        return mean_a < mean_b;
    });
}

//Definimos los nodos, que son los rectángulos que se encuentran en el árbol
//Contienen un vector de nodos hijos y un vector de rectángulos
struct Node {
    bool isLeaf;
    Rectangle rect;
    vector<Node*> children;
    vector<Rectangle> rects;

};
//Función que recibe un rectángulo y lo hace un nodo
Node* makeNode(Rectangle rect) {
    Node* node = new Node;
    node->rect = rect;
    node->isLeaf = true;
    return node;
}
//Función que agrupa M nodos en un nodo padre
Node* makeParent(vector<Node*> children) {
    Node* node = new Node;
    node->isLeaf = false;
    node->children = children;
    //Se calcula el rectángulo que contiene a todos los hijos
    int x1 = children[0]->rect.x1;
    int y1 = children[0]->rect.y1;
    int x2 = children[-1]->rect.x2;
    int y2 = children[-1]->rect.y2;
    Rectangle rect = {x1, y1, x2, y2};
    node->rect = rect;
    return node;
}
//Estructura R-tree
struct RTree {
    Node* root;
    int M;
};
//Función que recibe un vector de rectángulos y un entero M, y crea un R-tree
RTree* makeRTree(vector<Rectangle> rects, int M) {
    RTree* tree = new RTree;
    tree->M = M;
    //Se ordenan los rectángulos según la coordenada X del centro del rectángulo
    ordenarRectangulosX(rects);
    //Se crea un vector de nodos hoja
    vector<Node*> leafs;
    for (int i = 0; i < rects.size(); i++) {
        //Se crea un nodo hoja por cada rectángulo
        Node* node = makeNode(rects[i]);
        //Se agrega el nodo hoja al vector de nodos hoja
        leafs.push_back(node);
    }
    //Se crea un vector de nodos
    vector<Node*> nodes;
    //Se crea un nodo padre
    Node* parent;
    //Se crea un vector de nodos hijos
    vector<Node*> children;
    //Se crea un rectángulo que contiene a todos los rectángulos
    Rectangle rect = {leafs[0]->rect.x1, leafs[0]->rect.y1, leafs[-1]->rect.x2, leafs[-1]->rect.y2};
    //Se crea un nodo padre con el rectángulo que contiene a todos los rectángulos
    parent = makeNode(rect);
    //Se agrega el nodo padre al vector de nodos
    nodes.push_back(parent);
    //Se agrega el nodo padre al vector de nodos hijos
    children.push_back(parent);
    //Se agrega el primer nodo hoja al vector de nodos hijos
    children.push_back(leafs[0]);
    //Se agrega el segundo nodo hoja al vector de nodos hijos
    children.push_back(leafs[1]);
    //Se crea un nodo padre con los nodos hijos
    parent = makeParent(children);
    //Se agrega el nodo padre al vector de nodos
    nodes.push_back(parent);
    //Se agrega el nodo padre al vector de nodos hijos
    children.push_back(parent);
    //Se agrega el tercer nodo hoja al vector de nodos hijos
    children.push_back(leafs[2]);