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
    FILE* arch = fopen("leavesNX10.bin", "rb");
    if (arch == NULL){
        cout << "Error al abrir el archivo" << endl;
        return 1;
    }
    
    for(int i = 0; i<pow(2,10); i++) {
        Node *node = new Node;
        fread(node, sizeof(Node), 1, arch);
        cout << node->MBR.centerX() << " " << i << endl;
    }

    fclose(arch);
    return 0;

}