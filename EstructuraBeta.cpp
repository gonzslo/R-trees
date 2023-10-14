#include <vector>

struct Rectangle {
    // Coordenadas de la esquina inferior izquierda
    float x1, y1;
    // Coordenadas de la esquina superior derecha
    float x2, y2;
};

struct Node {
    Rectangle rect; // Rectángulo que contiene a todos los rectángulos hijos
    std::vector<Node*> children; // Hijos del nodo
    std::vector<Rectangle> data; // Rectángulos hijos
};

class RTree {
public:
    RTree() {
        root = nullptr;
    }

  
    //Construcción mediante Nearest-X
    /*Dado un conjunto R de rectángulos, y M la máxima cantidad de hijos de cada
    nodo del árbol, el método ordena los rectángulos de R según la coordenada X
    del centro del rectángulo, para luego juntarlos en grupos de tamaño M, usando
    rectángulos consecutivos en la secuencia ordenada, para con ellos formar n/M
    nodos  con M hijos cada uno. De estos n/M nodos, identificados por rectángulos,
    tomamos nuevamente el centro y repetimos la anterior forma de agrupar  los
    puntos con este nuevo conjunto de nodos. Esto se repite hasta que se puedan agrupar
    todos los nodos de una iteración en un solo nodo raíz.*/
    void build(std::vector<Rectangle> rects, int M) {
    // Ordenar los rectángulos según la coordenada X del centro del rectángulo
    std::sort(rects.begin(), rects.end(), [](const Rectangle& a, const Rectangle& b) {
        float mean_a = (a.x1 + a.x2) / 2.0f;
        float mean_b = (b.x1 + b.x2) / 2.0f;
        return mean_a < mean_b;
    });
    //Impresión de los rectángulos ordenados
    std::cout << "Rectángulos ordenados según la coordenada X del centro del rectángulo: " << std::endl;
    for (int i = 0; i < rects.size(); i++) {
        std::cout << "Rectángulo " << i << ": " << std::endl;
        std::cout << "x1: " << rects[i].x1 << std::endl;
        std::cout << "y1: " << rects[i].y1 << std::endl;
        std::cout << "x2: " << rects[i].x2 << std::endl;
        std::cout << "y2: " << rects[i].y2 << std::endl;
    }


    
    // TODO: implement R-tree construction algorithm
}
    
        

        
    void insert(Rectangle rect) {
        -// TODO: implement insertion algorithm
    }
    void remove(Rectangle rect) {
        // TODO: implement deletion algorithm
    }

    std::vector<Rectangle> search(Rectangle rect) {
        std::vector<Rectangle> results;
        if (root != nullptr) {
            // TODO: implement search algorithm
        }
        return results;
    }

private:
    Node* root;
};
