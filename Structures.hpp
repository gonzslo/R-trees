#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <random>
#include <cmath>

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


void ordenarRectangulosX(vector<Rectangle> &rects){
    sort(rects.begin(), rects.end(), [](const Rectangle &a, const Rectangle &b){
        float mean_a = (a.x1 + a.x2) / 2.0f;
        float mean_b = (b.x1 + b.x2) / 2.0f;
        return mean_a < mean_b; });
}
Node *makeLeaf(Rectangle rect){
    Node *node = new Node;
    node->MBR = rect;
    node->isLeaf = true;
    return node;
}
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
vector<Node *> makeLeaves(vector<Rectangle> rects){
    vector<Node *> leaves;
    ordenarRectangulosX(rects);
    for (int i = 0; i < rects.size(); i++){
        Node *node = makeLeaf(rects[i]);
        leaves.push_back(node);
    }
    return leaves;
}
RTree makeGroups(vector<Node *> children, int M, int factor, int nivel){
    string nombre = "groupsNX" + to_string(factor) +"Nivel" + to_string(nivel) + ".bin";
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
        //cout << "Nodo grupito: " << node->MBR.x1 << ", " << node->MBR.x2 << endl;
        // for (int k = 0; k < grupito.size(); k++){
        //     //cout << "Nodo hoja: " << grupito[k]->MBR.x1 << ", " << grupito[k]->MBR.x2 << endl;
        // }
        grupito.clear();
    }
    FILE *arch = fopen(nombre.c_str(), "wb");
    if (arch == NULL){
        cout << "Error al abrir el archivo" << endl;
        return RTree();
    }
    for (int i = 0; i < grupo.size(); i++){
        fwrite(grupo[i], sizeof(Node), 1, arch);
    }
    fclose(arch);

    return makeGroups(grupo, M, factor, nivel-1);
}
RTree nearestX(vector<Rectangle> rects, int M, int factor){
    vector<Node *> leaves = makeLeaves(rects);
    int nivel = ceil((log10(pow(2,  factor)) / log10(1024))) ;
    string nombre = "binNX/groupsNX" +to_string(factor)+ "Nivel" + to_string(nivel + 1) + ".bin";

    FILE *arch = fopen(nombre.c_str(), "wb");
    if (arch == NULL){
        cout << "Error al abrir el archivo" << endl;
        return RTree();
    }
    for (int i = 0; i < leaves.size(); i++){
        fwrite(leaves[i], sizeof(Node), 1, arch);
    }

    fclose(arch);

    
    RTree rtree = makeGroups(leaves, M, factor, nivel);
    return rtree;
}