#include "Structures.hpp"
using namespace std;


// Se crea función que convierte un punto a su representación en la curva de Hilbert
//convierte (x,y) a d
//rotar/voltear un cuadrante apropiadamente
void rot(int n, int *x, int *y, int rx, int ry) {
    int t;
    if (ry == 0) {
        if (rx == 1) {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }
        t  = *x;
        *x = *y;
        *y = t;
    }
}
int xy2d (int n, int x, int y) {
    int rx, ry, s, d=0;
    for (s = n/2; s > 0; s /= 2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    }
    return d;
}

// Se crea función que ordena los centros de los rectángulos en función
// de su valor dentro de la curva de Hilbert

void ordenarRectangulosHilbert(vector<Rectangle> &rects){
    sort(rects.begin(), rects.end(), [](const Rectangle &a, const Rectangle &b){
        int n = 4;
        float x1 = (a.x2+a.x1)/2.0f;
        float y1 = (a.y2+a.y1)/2.0f;
        float x2 = (b.x2+b.x1)/2.0f;
        float y2 = (b.y2+b.y1)/2.0f;

        int d1 = xy2d(n, x1, y1);
        int d2 = xy2d(n, x2, y2);

        return d1 < d2; });
}
vector<Node *> makeLeavesH(vector<Rectangle> rects){
    vector<Node *> leaves;
    ordenarRectangulosHilbert(rects);
    for (int i = 0; i < rects.size(); i++){
        Node *node = makeLeaf(rects[i]);
        leaves.push_back(node);
    }
    return leaves;
}
RTree makeGroupsH(vector<Node *> children, int M, int factor, int nivel){
    string nombre = "binHilbert/groupsHT" + to_string(factor) +"Nivel" + to_string(nivel) + ".bin";
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
    return makeGroupsH(grupo, M, factor, nivel-1);
}
RTree HilbertRTree(vector<Rectangle> rects, int M, int factor){
    vector<Node *> leaves = makeLeaves(rects);
    int nivel = ceil((log10(pow(2,  factor)) / log10(M))) ;
    string nombre = "binHilbert/groupsHT" +to_string(factor)+ "Nivel" + to_string(nivel + 1) + ".bin";

    FILE *arch = fopen(nombre.c_str(), "wb");
    if (arch == NULL){
        cout << "Error al abrir el archivo" << endl;
        return RTree();
    }
    for (int i = 0; i < leaves.size(); i++){
        fwrite(leaves[i], sizeof(Node), 1, arch);
    }
    fclose(arch);
    RTree rtree = makeGroupsH(leaves, M, factor, nivel);
    return rtree;
}
