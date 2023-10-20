#include "Structures.hpp"
#include<chrono>
#include <fstream>
using namespace std;

bool intersects(const Rectangle& rect1, const Rectangle& rect2) {
    // Verifica si dos rectángulos se intersectan.
    if (rect1.x1 > rect2.x2 || rect2.x1 > rect1.x2) {
        return false;
    }
    if (rect1.y1 > rect2.y2 || rect2.y1 > rect1.y2) {
        return false;
    }
    return true;
}
int nodosporleerF(int factor, int nivel, int M) {
    nivel--;
    int nodosporleer=pow(2,nivel);
    if (nivel>=1){
        nodosporleer=pow(2,factor%10+10*(nivel-1));
        if(factor%10==0){
            nodosporleer=pow(2,10*nivel);
        }
    }
    return nodosporleer;
}
int nodosFuturosF(int factor, int nivel, int M){
    return nodosporleerF(factor, nivel+1, M)/nodosporleerF(factor, nivel, M);
}

vector<Rectangle>searchRecursive(const Rectangle& value, vector<bool> offset,
                                 int nivel, int factor, string filename, int M, int& diskAccesses) {

    int nivelfinal = ceil((log10(pow(2,  factor)) / log10(M)))+1;
    vector<Rectangle> tempResult = vector<Rectangle>();
    vector<bool> offset2 = vector<bool>();

    if(nivel == nivelfinal) { // Si estamos en el último nivel (en las hojas del arbol)
        FILE* arch = fopen(filename.c_str(), "rb");
        if (arch == NULL){
            cout << "Error al abrir el archivo" << endl;
            return tempResult;
        }
        for (int i = 0; i < pow(2, factor); i++){
            Node *node = new Node;
            fread(node, sizeof(Node), 1, arch);
            diskAccesses++;
            if(intersects(node->MBR, value)){
                tempResult.push_back(node->MBR);
            }
                
        }
        fclose(arch);
        return tempResult; // Caso base porque estamos al final
    }
    else {//Inspecciona si los mbr de cada nodo intersectan con value. Si es así, se busca recursivamente en el siguiente nivel
        FILE* arch = fopen(filename.c_str(), "rb");
        if (arch == NULL){
            cout << "Error al abrir el archivo" << endl;
            return tempResult;
        }
        int nodosFuturos=nodosFuturosF(factor, nivel, M);
        for (int i = 0; i <= offset.size(); i++){
            Node *node = new Node;
            if (offset[i]==false){
                fseek(arch, sizeof(Node)*nodosFuturos, SEEK_CUR);}
            else{
                for(int j=0; j<nodosFuturos; j++){
                    fread(node, sizeof(Node), 1, arch);
                    diskAccesses++;
                    if(intersects(node->MBR, value)){
                        offset2.push_back(true);
                    }
                    else{
                        offset2.push_back(false);
                    }
                }
            }
        }
        fclose(arch);
        string filename = "binNX/groupsNX" +to_string(factor)+ "Nivel" + to_string(nivel+1) + ".bin";
        return searchRecursive(value, offset2, nivel+1, factor, filename, M, diskAccesses);
    }
}
// Búsqueda: retorna un vector de rectangulos que intersectan con value
vector<Rectangle> search(const Rectangle& value, int factor, int M, int& diskAccesses) {
    vector<Rectangle> result = vector<Rectangle>();
    vector<bool> offset = vector<bool>();
    int nivel = 1;
    string filename = "binNX/groupsNX" +to_string(factor)+ "Nivel" + to_string(nivel) + ".bin";
    FILE* arch = fopen(filename.c_str(), "rb");
    if (arch == NULL){
        cout << "Error al abrir el archivo" << endl;
        return result;
    }
    Node* node = new Node;
    fread(node, sizeof(Node), 1, arch);
    diskAccesses++;
    fclose(arch);


    //Si el MBR de la raíz intersecta con el rectángulo, se busca recursivamente para añadir los nodos que intersectan a result
    if(intersects(node->MBR, value))
        nivel++;
        filename = "binNX/groupsNX" +to_string(factor)+ "Nivel" + to_string(nivel) + ".bin";
        offset.push_back(true);
        vector<Rectangle> result2 = searchRecursive(value, offset, nivel, factor, filename, M, diskAccesses);
        for (int i = 0; i < result2.size(); i++){
            result.push_back(result2[i]);
        }
        
    return result;
}

int main(){
    int diskAccesses;
    int M = 1024;

    ofstream results("resultados.txt");
    if(results.is_open()){

        results << "Arbol,";
        for (int k=0; k<=100; k++){
            results << "Tiempo "<< k << "," << "Disk Accesses "<< k << ",";
        }
        results << endl;

        //Almacenamos en memoria el vector de rectángulos Q alojado en disco
        vector<Rectangle> rectsQ = vector<Rectangle>();
        FILE* arch = fopen("rectsQ.bin", "rb");
        if (arch == NULL){
            cout << "Error al abrir el archivo" << endl;
            return 0;
        }
        for (int i = 0; i<=100; i++){
            Rectangle rect;
            fread(&rect, sizeof(Rectangle), 1, arch);
            rectsQ.push_back(rect);
        }
        fclose(arch);


        for (int factor = 10; factor<=25; factor++){
            cout << "Búsqueda en factor " << factor << endl;
            results << factor << ",";
            
            //Para obtener todos los resultados y no morir en el intento, se recomienda leer los rectángulos de 10 en 10 por cada corrida,
            //y salvar los resultados. Así la recursión no sobresatura la ram
            for (int i = 0; i<=100; i++){
                diskAccesses = 0;
                auto start = chrono::high_resolution_clock::now();
                search(rectsQ[i], factor, M, diskAccesses);
                auto stop = chrono::high_resolution_clock::now();
                auto tiempofinal = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
                results << tiempofinal << "," << diskAccesses << ",";

            }
            results << endl;

        }
    }
    results.close();
    return 0;
}