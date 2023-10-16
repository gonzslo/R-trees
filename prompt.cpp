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
#include "algoritmo1.hpp"
using namespace std;

const string RTreeFile = "rtree.bin";
int lastBlock = 0;

void saveNodeToFile(Node *node, ofstream &out) {
    out.write((char*) node, sizeof(Node));
    for(Node* child : node->children) {
        saveNodeToFile(child, out);
    }
}

void saveRTreeToFile(RTree &tree) {
    ofstream out(RTreeFile, ios::binary);
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

RTree readRTreeFromFile(int &diskAccesses) {
    ifstream in(RTreeFile, ios::binary);
    RTree tree;
    tree.root = readNodeFromFile(in, diskAccesses);
    in.close();
    return tree;
}

// 2. Ajustar búsqueda para contar accesos a disco

// Actualizaremos el método searchRecursive para contar los accesos a disco.

// cpp

void searchRecursive(Node* currentNode, const Rectangle& value, std::vector<Rectangle>& result, int &diskAccesses) {
    if(currentNode == nullptr)
        return;
    if(currentNode->isLeaf) {
        bool currentIntersects = (*currentNode).intersects(currentNode->MBR, value);
        if (currentIntersects) {
            result.push_back(currentNode->MBR);
        }
    } else {
        for (Node* child : currentNode->children) {
            diskAccesses++;
            if((*child).intersects(child->MBR, value)) {
                searchRecursive(child, value, result, diskAccesses);
            }
        }
    }
}

// 3. Implementar función de pruebas

// cpp

#include <chrono>
#include <cmath>
#include <ostream>
using namespace std;

void performTests() {
    for(int i = 10; i <= 25; i++) {
        int size = pow(2, i);
        vector<Rectangle> rects;
        // Aquí debes agregar el código para generar los Rectángulos

        // Creamos el R-tree
        RTree rtree = nearestX(rects, size);

        // Guardamos el R-tree en disco

        void performTests1() {
            for(int i = 10; i <= 25; i++) {
                int size = pow(2, i);
                vector<Rectangle> rects;
                // Aquí debes agregar el código para generar los Rectángulos

                // Creamos el R-tree
                RTree rtree = nearestX(rects, size);

                // Guardamos el R-tree en disco
                saveRTreeToFile(rtree);

                // Realizamos las consultas
                int totalDiskAccesses = 0;
                auto start = std::chrono::high_resolution_clock::now(); // declare start variable here
                for(int j = 0; j < 100; j++) {
                    int diskAccesses = 0;
                    Rectangle query; // Define tu consulta aquí
                    RTree diskTree = readRTreeFromFile(diskAccesses);
                    vector<Rectangle> results;
                    diskTree.search(query, results, diskAccesses);
                    totalDiskAccesses += diskAccesses;
                }
                auto finish = std::chrono::high_resolution_clock::now();

                auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);
                cout << "Size: " << size << ", Time: " << elapsed.count() << ", Disk Accesses (avg): " << totalDiskAccesses / 100.0 << endl;
            }
        }
    }
}

// Este código te brinda una idea de cómo puedes estructurar tu solución, 
// pero es probable que necesites hacer ajustes para que funcione exactamente como lo necesitas.
// También es importante tener en cuenta la eficiencia y la gestión de memoria cuando trabajas con estructuras
// de datos grandes como los R-trees. ¡Buena suerte!
