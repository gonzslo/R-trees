#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include<algorithm>
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
    float centerY(){
        return (y1 + y2) / 2.0f;
    }
    void print(){
        cout << "x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << endl;
    }
};

// Definimos los nodos, que son los rectángulos que se encuentran en el árbol
// Contienen un vector de nodos hijos y un vector de rectángulos
struct Node{
    bool isLeaf;
    Rectangle MBR;
    vector<Node *> children;
    bool intersects(const Rectangle& rect1, const Rectangle& rect2) {
        // Verifica si dos rectángulos se intersectan.
        bool x_overlap = (rect1.x1 <= rect2.x2 && rect1.x2 >= rect2.x1) || (rect2.x1 <= rect1.x2 && rect2.x2 >= rect1.x1);
        bool y_overlap = (rect1.y1 <= rect2.y2 && rect1.y2 >= rect2.y1) || (rect2.y1 <= rect1.y2 && rect2.y2 >= rect1.y1);

        return x_overlap && y_overlap;
    }
};

// Estructura R-Tree
struct RTree{
    Node *root;
    int M;
    vector<Node *> hijos;
    // Utiliza funciones auxiliares: intersects, searchRecursive
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
    // Búsqueda: retorna un vector de rectangulos que intersectan con value
    vector<Rectangle> search(const Rectangle& value) {
        vector<Rectangle> result;
        searchRecursive(root, value, result);
        return result;
    }
    //Método para imprimir un R-tree
    void print(Node *root){
        if(root == nullptr)
            return;
        if(root->isLeaf){
            cout << "Leaf: " << root->MBR.x1 << " " << root->MBR.y1 << " " << root->MBR.x2 << " " << root->MBR.y2 << endl;
        }else{
            cout << "Node: " << root->MBR.x1 << " " << root->MBR.y1 << " " << root->MBR.x2 << " " << root->MBR.y2 << endl;
            for(int i = 0; i < root->children.size(); i++){
                print(root->children[i]);
            }
        }
    }




};

void ordenarRectangulosX(vector<Rectangle> &rects);
Node *makeLeaf(Rectangle rect);
Node *makeParent(vector<Node *> children);
vector<Node *> makeLeaves(vector<Rectangle> rects);
Node *makeLeaf(Rectangle rect);
RTree makeGroups(vector<Node *> children, int M);
RTree nearestX(vector<Rectangle> rects, int M);