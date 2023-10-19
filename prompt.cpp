// Entendido. Ahora, para cumplir con tu solicitud, necesitamos:

//     Guardar el R-tree en disco.
//     Realizar consultas en el R-tree desde el disco.
//     Registrar el tiempo de consulta y contar los accesos a bloques de disco.
//     Repetir esto para varios tamaños de R-tree.

// Para hacerlo más manejable, seguiré estos pasos:

//     Definir una función que guarde y lea el R-tree en disco.
//     Ajustar la búsqueda para que cuente los accesos a bloques de disco.
//     Implementar una función que realice las pruebas.

// Vamos a realizarlo paso a paso:
// 1. Función para guardar y leer el R-tree en disco

// Para simplificar, usaremos archivos binarios para guardar y leer el R-tree. 
// Asumiremos que cada nodo (y su información) se guarda en un "bloque" y cada lectura de un bloque desde el archivo será contada como un acceso a disco.

// cpp

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <random>
#include <math.h>
#include "Structures.hpp"
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
    // Se crean las hojas
    vector<Node *> leaves = makeLeaves(rects);
    // Se crea el R-tree
    RTree rtree = makeGroups(leaves, M);
    return rtree;
}

//Se dejan 17 strings para nombrar los archivos de los 16 árboles
vector<string> RTreeFile = {"rtree1.bin", "rtree2.bin", "rtree3.bin", "rtree4.bin", "rtree5.bin",
                            "rtree6.bin", "rtree7.bin", "rtree8.bin", "rtree9.bin", "rtree10.bin",
                            "rtree11.bin", "rtree12.bin", "rtree13.bin", "rtree14.bin", "rtree15.bin",
                            "rtree16.bin", "rtree17.bin"};


int lastBlock = 0;

void saveNodeToFile(Node *node, ofstream &out) {
    out.write((char*) node, sizeof(Node));
    for(Node* child : node->children) {
        saveNodeToFile(child, out);
    }
}

void saveRTreeToFile(RTree &tree, int i) {
    ofstream out(RTreeFile[i], ios::binary);
    lastBlock = 0;
    saveNodeToFile(tree.root, out);
    out.close();
}

Node* readNodeFromFile(ifstream &in, int &diskAccesses) {
    Node* node = new Node;
    in.read((char*) node, sizeof(Node));
    diskAccesses++;
    for(int i = 0; i < node->children.size(); i++) {
        node->children[i] = readNodeFromFile(in, diskAccesses);
    }
    return node;
}

RTree readRTreeFromFile(int &diskAccesses, int i) {
    ifstream in("rtree150.bin", ios::binary);
    RTree tree;
    tree.root = readNodeFromFile(in, diskAccesses);
    in.close();
    return tree;
}

Node* readOneNodeFromFile(ifstream &in, int &diskAccesses){
    Node* node = new Node;
    in.read((char*) node, sizeof(Node));
    diskAccesses++;
    return node;
}
RTree readRootFromFile(int &diskAccesses, int i) {
    ifstream in(RTreeFile[i], ios::binary);
    RTree tree;
    tree.root = readOneNodeFromFile(in, diskAccesses);
    in.close();
    return tree;
}


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

    int j = pow(2,n); // Número de rectángulos
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
    // //Creamos 16 árboles con diferentes cantidades de rectángulos
    // for (int i=0; i<16; i++){
    //     //Se crea un vector de rectángulos
    //     vector<Rectangle> rects = createRandomRectangles(i+1);
    //     //Se crea el R-tree
    //     RTree rtree = nearestX(rects, 4096);
    //     //Se guarda el R-tree en un archivo binario
    //     saveRTreeToFile(rtree, i);
    // }
    // //Se leen los árboles desde los archivos binarios
    // for (int i=0; i<16; i++){
    //     int diskAccesses = 0;
    //     RTree rtree1 = readRTreeFromFile(diskAccesses, i);
    //     cout << "Accesos a disco: " << diskAccesses << endl;
    //     //imprime el arbol:
    // }
    
    //Creamos un arbol aleatoriamente para guardarlo en disco
    vector<Rectangle> rects = createRandomRectangles(3);
    RTree rtree = nearestX(rects, 3);
    saveRTreeToFile(rtree, 16);
    //rtree.print(rtree.root);


    //Leemos el arbol desde el archivo binario
    int diskAccesses = 0;
    RTree rtree150 = readRTreeFromFile(diskAccesses, 16);
    cout << "Accesos a disco: " << diskAccesses << endl;
    diskAccesses = 0;
    RTree rtree2 = readRootFromFile(diskAccesses, 16);
    rtree2.print(rtree2.root);
    cout << "Arbol original" <<endl;
    rtree150.print(rtree150.root);


    

    return 0;
}