#include "Structures.hpp"
using namespace std;

int main(){
    //leer todos los arboles de un factor
    int factor = 10;
    int nivel = ceil((log10(pow(2,  factor)) / log10(1024))) ;

    for(int i = 0; i<=nivel; i++){
        //Abrir el archivo que contiene al nodo
        string filename = "binNX/groupsNX" +to_string(factor)+ "Nivel" + to_string(i+1) + ".bin";
        cout << filename << endl;
        FILE* arch = fopen(filename.c_str(), "rb");
        if (arch == NULL){
            cout << "Error al abrir el archivo" << endl;
            return 1;
        }
        int nodosporleer=(pow(2,10*i));
        cout << nodosporleer << endl;
        if (nodosporleer>pow(2,factor)){
            nodosporleer=pow(2,factor);
        }
        cout <<"nodos por leer corregido "<< nodosporleer << endl;
        for (int i=0; i<nodosporleer; i++){
            Node *node = new Node;
            fread(node, sizeof(Node), 1, arch);
            cout <<i+1 <<" " <<node->MBR.centerX() << " " << node->MBR.centerY() << endl;
            
        }
        fclose(arch);

    }

    return 0;

}