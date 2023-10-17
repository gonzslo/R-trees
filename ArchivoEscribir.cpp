#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include<algorithm>
#include<algoritmo1.hpp>
#include<chrono>
#include <fstream>
#include <random>
using namespace std;

// Se crea función que recibe un vector de rectángulos y los ordena según la coordenada X del centro del rectángulo
void ordenarRectangulosX(vector<Rectangle> &rects){
    sort(rects.begin(), rects.end(), [](const Rectangle &a, const Rectangle &b){
        float mean_a = (a.x1 + a.x2) / 2.0f;
        float mean_b = (b.x1 + b.x2) / 2.0f;
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
            //cout << "Nodo hoja: " << grupito[k]->MBR.x1 << ", " << grupito[k]->MBR.x2 << endl;
        }
        grupito.clear();
    }
    return makeGroups(grupo, M);
}

// Función Nearest-X que construye un R-tree a partir de un vector de rectángulos y un parámetro M
RTree nearestX(vector<Rectangle> rects, int M){
    vector<Node *> leaves = makeLeaves(rects);
    RTree rtree = makeGroups(leaves, M);
    return rtree;
}

// Función que crea rectángulos aleatorios
vector<Rectangle> createRandomRectangles(int n){
    vector<Rectangle> rects;
    random_device rd;
    mt19937 gen(rd());

    // //Valores aleatorios entre 0 y 500mil
    // uniform_int_distribution<> distr1(0, 500000);
    // //El tamaño de los lados de los rectángulos es aleatorio entre 0 y 100
    // uniform_int_distribution<> distr2(0, 100);

    //Valores aleatorios entre 0 y 20
    uniform_int_distribution<> distr1(0, 100);
    //El tamaño de los lados de los rectángulos es aleatorio entre 0 y 10
    uniform_int_distribution<> distr2(0, 10);

    int j = pow(2,n+9); // Número de rectángulos
    for (int i=0; i<j; i++){
        int x1 = distr1(gen);
        int y1 = distr1(gen);
        int x2 = x1 + distr2(gen);
        int y2 = y1 + distr2(gen);
        if (x2>500000){
            x2 = 500000;
        }
        if (y2>500000){
            y2 = 500000;
        }
        Rectangle rect;
        rect.x1 = x1;
        rect.y1 = y1;
        rect.x2 = x2;
        rect.y2 = y2;

        rects.push_back(rect);

        //Imprime los rectángulos generados
        //cout << "Rectangulo " << i + 1 << ": " << x1 << ", " << x2 << " " << y1 << ", " << y2 << endl;
    }
    return rects;
}    


int main(){

    //Se crea un nodo con un rectángulo
    Rectangle rect = {4, 94, 51, 100};
    Rectangle rect2 = {1, 2, 3, 2};
    Node *node = makeLeaf(rect);
    Node *node2 = makeLeaf(rect2);
    node->MBR.print();
    node2->MBR.print();
    //Guardamos el nodo en disco
    FILE* arch = fopen("nodo.bin", "wb");
    if (arch == NULL){
        cout << "Error al abrir el archivo" << endl;
        return 1;
    }
    fwrite(node, sizeof(Node), 1, arch);
    fwrite(node2, sizeof(Node), 1, arch);
    fclose(arch);

    
}