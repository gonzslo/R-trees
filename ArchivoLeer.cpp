#include "Structures.hpp"
using namespace std;
#include <fstream>

int main(){
    //leer todos los arboles de un factor
    int factor = 12;
    int nivel = ceil((log10(pow(2,  factor)) / log10(1024))) ;
    ofstream results("resultados.txt");
    if(results.is_open()){
        for(int i = 0; i<=nivel; i++){
            //Abrir el archivo que contiene al nodo
            string filename = "binNX/groupsNX" +to_string(factor)+ "Nivel" + to_string(i+1) + ".bin";
            cout << filename << endl;
            results << filename << endl;
            FILE* arch = fopen(filename.c_str(), "rb");
            if (arch == NULL){
                cout << "Error al abrir el archivo" << endl;
                return 1;
            }
            int nodosporleer=pow(2,i);
            if (i>=1){
                nodosporleer=pow(2,factor%10+10*(i-1));
                if(factor%10==0){
                    nodosporleer=pow(2,10*i);
                }
            }
            results <<"nodos por leer corregido "<< nodosporleer << endl;
            cout <<"nodos por leer corregido "<< nodosporleer << endl;
            for (int i=0; i<nodosporleer; i++){
                Node *node = new Node;
                fread(node, sizeof(Node), 1, arch);        
            }
            fclose(arch);

        }
    }
    return 0;

}
