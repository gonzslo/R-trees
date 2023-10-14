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
    return node;
}
//Función que recibe un vector de rectángulos ordenados, y los agrupa en 
//vectores de tamaño M. Retorna un vector de vectores de rectángulos
vector<Node> agruparRectangulos(vector<Rectangle> *rects, int M){
    vector<Node> grupos;
    vector<Node> grupo;
    int i = 0;
    for (auto rect : rects){
        nodoHoja = makeNode(rect);
        
        if (i == M){
            grupos.push_back(grupo);
            grupo.clear(); //
            i = 0;
        }
        grupo.push_back(rect);
        i++;
    }
    grupos.push_back(grupo);
    return grupos;
}

//

