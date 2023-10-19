#include "Structures.hpp"
using namespace std;
#include <fstream>

bool intersects(const Rectangle& rect1, const Rectangle& rect2) {
    // Verifica si dos rectángulos se intersectan.
    bool x_overlap = (rect1.x1 <= rect2.x2 && rect1.x2 >= rect2.x1) || (rect2.x1 <= rect1.x2 && rect2.x2 >= rect1.x1);
    bool y_overlap = (rect1.y1 <= rect2.y2 && rect1.y2 >= rect2.y1) || (rect2.y1 <= rect1.y2 && rect2.y2 >= rect1.y1);

    return x_overlap && y_overlap;
}
void searchRecursive(Node* currentNode, const Rectangle& value,
                     std::vector<Rectangle>& result,int nivel, 
                     int factor, string filename, FILE *file) {

    if(filename == "binNX/groupsNX" +to_string(factor)+ "Nivel" + to_string(nivel) + ".bin") { // Si estamos en el último nivel (en las hojas del arbol)
        result.push_back(currentNode->MBR); // Caso base porque estamos al final
    }
    else { // Recursión
       
         // Hay que cambiar la iteración por búsqueda en el siguiente archivo en el puntero del hijo del nodo MBR que intersecta
        if(intersects(currentNode->MBR, value)) { // Avanzamos en el nivel desde el primer MBR que intersecta hasta el último
            
        } else {
            fseek(file, sizeof(Node), SEEK_CUR); // Si el nodo intersecta, vamos al puntero hacia el hijo que está en el siguiente archivo y buscamos recursivamente en el
            searchRecursive(currentNode, value, result, nivel+1, factor, filename, file);
        }
    }
}
// Búsqueda: retorna un vector de rectangulos que intersectan con value
vector<Rectangle> search(Node* node, const Rectangle& value, int nivel, int factor, string filename, FILE *file) {
    vector<Rectangle> result;
    if(!intersects(node->MBR, value))
        return vector<Rectangle>();
    searchRecursive(node, value, result, nivel, factor, filename, file);
    return result;
}

int main(){
    //leer todos los arboles de un factor
    Rectangle value = {1,2,3,4}; //Solo para probar
    ofstream results("resultados.txt");
    if(results.is_open()){
        for(int factor=10; factor<=10; factor++){
            int nivel = ceil((log10(pow(2,  factor)) / log10(1024))) ;
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
                results << "nodos por leer corregido "<< nodosporleer << endl;
                cout << "nodos por leer corregido "<< nodosporleer << endl;
                for (int i=0; i<nodosporleer; i++){ //Leer todos los nodos del archivo
                    Node *node = new Node;
                    fread(node, sizeof(Node), 1, arch);

                }
                fclose(arch);
            }
        }
    }
    return 0;
}

