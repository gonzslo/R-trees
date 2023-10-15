#include <vector>
using namespace std;

struct Rectangle{
    int x1, y1;
    int x2, y2;

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
};
struct Node{
    bool isLeaf;
    Rectangle MBR;
    vector<Node *> children;
};
struct RTree{
    Node *root;
    int M;
    vector<Node *> hijos;
    // Búsqueda (lo que se busca es un rectángulo)
    std::vector<Rectangle> search(const Rectangle& value) { // retorna 1 si está y 0 si no
        std::vector<Rectangle> result;
        Node* currentNode = root;
        if(currentNode == nullptr) return result;
        if(currentNode->isLeaf) {
            
        }
    }
};

void ordenarRectangulosX(vector<Rectangle> &rects);
Node* makeLeaf(Rectangle rect);
Node* makeParent(vector<Node*> children);
vector<Node*> makeLeaves(vector<Rectangle> rects);
RTree makeGroups(vector<Node*> children, int M);

