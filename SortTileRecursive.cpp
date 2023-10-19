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
void ordenarNodosX(vector<Node *> &rects){
    sort(rects.begin(), rects.end(), [](const Node *a, const Node *b){
        float mean_a = (a->MBR.x1 + a->MBR.x2) / 2.0f;
        float mean_b = (b->MBR.x1 + b->MBR.x2) / 2.0f;
        return mean_a < mean_b; });
}

// Ordena rectangulos/puntos segun su coordenada Y
void ordenarNodosY(vector<Node *> &rects){
    sort(rects.begin(), rects.end(), [](const Node *a, const Node *b){
        float mean_a = (a->MBR.y1 + a->MBR.y2) / 2.0f;
        float mean_b = (b->MBR.y1 + b->MBR.y2) / 2.0f;
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
vector<Node *> makeLeavesNormal(vector<Rectangle> rects){
    vector<Node *> leaves;
    for (int i = 0; i < rects.size(); i++){
        Node *node = makeLeaf(rects[i]);
        leaves.push_back(node);
    }
    return leaves;
}

// Función que crea hojas a partir de un vector de rectángulos que primero ordena la coordenada Y
vector<Node *> makeLeavesY(vector<Node *> rects){
    vector<Node *> leaves;
    ordenarNodosY(rects);
    for (int i = 0; i < rects.size(); i++){
        leaves.push_back(rects[i]);
    }
    return leaves;
}

// Función que recibe un vector de puntos y hace grupos de tamaño sqrt(n/M)
vector<vector<Node *>> makeXGroups(vector<Node *> points, int M){
    ordenarNodosX(points); // Se ordenan los puntos segun su coordenada X
    vector<vector<Node *>> grupo; // grupo es vector de vectores de puntos
    for (int i = 0; i < (int)ceil(sqrt((double)points.size() / M)); i++){
        vector<Node *> grupito; // grupito es vector de puntos
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
vector<Node *> makeYGroups(vector<vector<Node *>> XGroups, int M){
    vector<Node *> grupo; // este grupo es vector de nodos
    for (int i = 0; i < XGroups.size(); i++){ // hay que crear un rectangulo que contenga los puntos
        vector<Node *> points_leaves = makeLeavesY(XGroups[i]); // convertir puntos en hojas
        vector<Node *> grupito; // grupito es vector de nodos
        for (int j = 0; j < (int)ceil((double)XGroups[i].size()/M); j++) { // sqrt(n/M) nodos
            for(int k = 0; k < M; k++) {
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
    return grupo; // vector con todos los MBR que contienen a los puntos (nodos)
}

RTree makeGroupsRecursive(vector<Node *> leaves, int M, int factor, int nivel) {
    string nombre = "binNX/groupsNX" + to_string(factor) +"Nivel" + to_string(nivel) + ".bin";
    // Recursión del árbol (caso base)
    if (leaves.size() == 1) {
        RTree rtree = RTree();
        rtree.hijos = leaves;
        rtree.root = leaves[0];
        return rtree;
    }
    // Se ordenan los puntos segun su coordenada X
    // y se hacen sqrt(n/M) grupos
    vector<vector<Node *>> XGroups = makeXGroups(leaves, M);
    // Se ordenan los puntos de cada grupo según su coordenada Y
    // y se hacen M grupos dentro de cada XGroup
    vector<Node *> new_leaves = makeYGroups(XGroups, M); // Recién en este punto se tienen hojas
    // Archivos:
    FILE *arch = fopen(nombre.c_str(), "wb");
    if (arch == NULL){
        cout << "Error al abrir el archivo" << endl;
        return RTree();
    }
    for (int i = 0; i < new_leaves.size(); i++){
        fwrite(new_leaves[i], sizeof(Node), 1, arch);
    }
    fclose(arch);
    return makeGroupsRecursive(new_leaves, M, factor, nivel-1);
}

// Función Sort-Tile-Recursive que construye un R-tree
RTree SortTileRecursive(vector<Rectangle> points, int M, int factor) {
    // Transforma los puntos en hojas
    vector<Node *> leaves = makeLeavesNormal(points);
    // Escribir el arbol en archivos binarios
    int nivel = ceil((log10(pow(2,  factor)) / log10(1024))) ;
    string nombre = "binSTR/groupsNX" +to_string(factor)+ "Nivel" + to_string(nivel + 1) + ".bin";
    FILE *arch = fopen(nombre.c_str(), "wb");
    if (arch == NULL){
        cout << "Error al abrir el archivo" << endl;
        return RTree();
    }
    for (int i = 0; i < leaves.size(); i++){
        fwrite(leaves[i], sizeof(Node), 1, arch);
    }
    fclose(arch);
    // Se llama hacen grupos recursivamente
    RTree rtree = makeGroupsRecursive(leaves, M);
    return rtree;
}
