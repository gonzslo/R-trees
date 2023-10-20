#include "Structures.hpp"
using namespace std;
#include <fstream>

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
                                 int nivel, int factor, string filename, int M) {

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
        return searchRecursive(value, offset2, nivel+1, factor, filename, M);
    }
}
// Búsqueda: retorna un vector de rectangulos que intersectan con value
vector<Rectangle> search(const Rectangle& value, int factor, int M) {
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
    fclose(arch);

    //Si el MBR de la raíz intersecta con el rectángulo, se busca recursivamente para añadir los nodos que intersectan a result
    if(intersects(node->MBR, value))
        cout << "intersecta"<< endl;
        nivel++;
        filename = "binNX/groupsNX" +to_string(factor)+ "Nivel" + to_string(nivel) + ".bin";
        offset.push_back(true);
        vector<Rectangle> result2 = searchRecursive(value, offset, nivel, factor, filename, M);
        for (int i = 0; i < result2.size(); i++){
            result.push_back(result2[i]);
        }
        
    return result;
}

int main(){
    int factor = 10;
    int M = 1024;
    //Crea un rectángulo aleatorio entre los rangos dados
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr1(0, 400000);
    uniform_int_distribution<> distr2(0, 100000);
    int x1 = distr1(gen);
    int y1 = distr1(gen);
    int x2 = x1 + distr2(gen);
    int y2 = y1 + distr2(gen);
    if (x2>500000){
        x2 = 500000;
    }
    if (y2>500000){
        y2 = 500000;
    }
    Rectangle value = {x1, y1, x2, y2};
    cout << "Rectángulo: " << value.x1 << " " << value.y1 << " " << value.x2 << " " << value.y2 << endl;


    vector<Rectangle> final = search(value, factor, M);

    ofstream results("resultados.txt");
    if(results.is_open()){
        for (int i = 0; i < final.size(); i++){
            results << final[i].x1 << " " << final[i].y1 << " " << final[i].x2 << " " << final[i].y2 << endl;
        }
    }

    
    return 0;
}