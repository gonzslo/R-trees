#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "Structures.hpp"
#include <chrono>
#include <fstream>
#include <random>
using namespace std;

using namespace std;

int main(){
    //Abrir el archivo que contiene al nodo
    FILE* arch = fopen("groupsNX10Nivel2.bin", "rb");
    if (arch == NULL){
        cout << "Error al abrir el archivo" << endl;
        return 1;
    }
    
    for (int i=0; i<1030; i++){
        Node *node = new Node;
        fread(node, sizeof(Node), 1, arch);
        cout <<i+1 <<" " <<node->MBR.centerX() << " " << node->MBR.centerY() << endl;

    }
    

    fclose(arch);
    return 0;

}