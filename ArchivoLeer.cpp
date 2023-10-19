#include "Structures.hpp"
using namespace std;

int main(){
    //Abrir el archivo que contiene al nodo
    FILE* arch = fopen("binSTR/groupsSTR20Nivel1.bin", "rb");
    if (arch == NULL){
        cout << "Error al abrir el archivo" << endl;
        return 1;
    }
    
    for (int i=0; i<10; i++){
        Node *node = new Node;
        fread(node, sizeof(Node), 1, arch);
        node->MBR.print();
    }
    

    fclose(arch);
    return 0;

}