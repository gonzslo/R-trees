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
RTree makeGroups(vector<Node *> children, int M, int factor, int nivel){
    string nombre = "binNX/groupsNX" + to_string(factor) +"Nivel" + to_string(nivel) + ".bin";
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

// Función Nearest-X que construye un R-tree a partir de un vector de rectángulos y un parámetro M
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