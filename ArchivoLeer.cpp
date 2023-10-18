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
    FILE* arch = fopen("leavesNX1.bin", "rb");
    if (arch == NULL){
        cout << "Error al abrir el archivo" << endl;
        return 1;
    }
    
    for (int i; i<2; i++){
        Node *node = new Node;
        fread(node, sizeof(Node), 1, arch);
        node->MBR.print();
        

    }
    

    fclose(arch);
    return 0;

}