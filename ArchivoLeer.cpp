#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "algoritmo1.hpp"
#include <chrono>
#include <fstream>
#include <random>
using namespace std;

using namespace std;

int main(){
    //Abrir el archivo que contiene al nodo
    FILE* arch = fopen("nodo.bin", "rb");
    if (arch == NULL){
        cout << "Error al abrir el archivo" << endl;
        return 1;
    }
    Node *node = new Node;
    Node *node2 = new Node;
    // fread(node, sizeof(Node), 1, arch);
    fseek(arch, sizeof(Node), SEEK_SET);
    fread(node2, sizeof(Node), 1, arch);

    fclose(arch);
    node->MBR.print();
    node2->MBR.print();
    return 0;

}