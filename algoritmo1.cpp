#include <vector>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <random>
#include <math.h>
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
    float centerX() {
        return (x1 + x2) / 2.0f;
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
    Rectangle MBR;
    vector<Node*> children;
};
//Función que recibe un rectángulo y lo hace un nodo
Node* makeLeaf(Rectangle rect) {
    Node* node = new Node;
    node->MBR = rect;
    node->isLeaf = true;
    return node;
}
//Función que agrupa M nodos en un nodo padre
Node* makeParent(vector<Node*> children) {
    Node* node = new Node;
    node->isLeaf = false;
    node->children = children;

    //Se calcula el rectángulo que contiene a todos los hijos
    int x_chiquito = children[0]->MBR.x1; // pivote
    int x_grande = children[0]->MBR.x2;
    int y_chiquito = children[0]->MBR.y1;
    int y_grande = children[0]->MBR.y2;
    for (int i = 0; i < children.size(); i++) {

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
//Estructura R-tree
struct RTree {
    Node* root;
    int M;
    vector<Node*> hijos;
};
//Función que crea hojas a partir de un vector de rectángulos que primero ordena
vector<Node*> makeLeaves(vector<Rectangle> rects) {
    vector<Node*> leaves;
    ordenarRectangulosX(rects);
    for (int i = 0; i < rects.size(); i++) {
        Node* node = makeLeaf(rects[i]);
        leaves.push_back(node);
    }
    return leaves;
}
//Función que recibe un vector de nodos y hace grupos 
RTree makeGroups(vector<Node*> children, int M){
    vector<Node*> grupo; //
    if (children.size()==1){
        RTree rtree = RTree();
        rtree.hijos=children;
        rtree.root=children[0];
        return rtree;
    }
    for (int i=0; i<(int)ceil((double)children.size()/M); i++){
        vector<Node*> grupito;
        for (int j=0; j<M; j++){
            int index = i*M+j;
            if (index >= children.size()){
                break;
            }
            grupito.push_back(children[index]);
        }
        Node* node = makeParent(grupito);
        
        grupo.push_back(node);
        cout << "Nodo grupito: " << node->MBR.x1 << ", " << node->MBR.x2 <<endl;
        for (int k=0; k<grupito.size(); k++){
            cout << "Nodo hoja: " << grupito[k]->MBR.x1 << ", " << grupito[k]->MBR.x2 <<endl;
        }
        grupito.clear();
    }
    return makeGroups(grupo, M);
}

//Main que genera aleatoriamente 6 rectángulos en un plano de 20x20 y los agrupa en un R-tree
//utiliza include random
int main(){
    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<> distr(0, 20);
    //Se crea un vector de rectángulos
    vector<Rectangle> rects;
    //Se crean 6 rectángulos aleatorios
    for (int i = 0; i < 7; i++) {
        int a = distr(eng);
        int b = distr(eng);
        int c = distr(eng);
        int d = distr(eng);
        int x1 = min(a, c);
        int x2 = max(a, c);
        int y1 = min(b, d);
        int y2 = max(b, d);
        Rectangle rect = {x1, y1, x2, y2};
        rects.push_back(rect);
        cout << "Rectangulo " << i+1 << ": " << x1 << ", " << x2 << " " << y1 << ", " << y2 << endl;

    }
    // Se hacen las hojas
    vector<Node*> leaves = makeLeaves(rects);
    for (int i=0; i<7; i++) {
        cout << "Centro: " << leaves[i]->MBR.centerX() << endl;
    }
    //Se crea el R-tree
    RTree rtree = makeGroups(leaves, 3);
    //Impresión de los nodos hoja
    cout << "Nodos hoja: " << endl;

    cout << "Nodo " << ": " << rtree.root->MBR.x1 << ", " << rtree.root->MBR.x2 <<endl;

  

}

