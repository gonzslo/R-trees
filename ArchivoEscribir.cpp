#include "Structures.hpp"
using namespace std;


// Función que crea rectángulos aleatorios
vector<Rectangle> createRandomRectangles(int n){
    vector<Rectangle> rects;
    random_device rd;
    mt19937 gen(rd());

    //Valores aleatorios entre 0 y 500mil
    uniform_int_distribution<> distr1(0, 500000);
    //El tamaño de los lados de los rectángulos es aleatorio entre 0 y 100
    uniform_int_distribution<> distr2(0, 100);

    // //Valores aleatorios entre 0 y 20
    // uniform_int_distribution<> distr1(0, 100);
    // //El tamaño de los lados de los rectángulos es aleatorio entre 0 y 10
    // uniform_int_distribution<> distr2(0, 10);

    int j = pow(2,n); // Número de rectángulos
    for (int i=0; i<j; i++){
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
        Rectangle rect;
        rect.x1 = x1;
        rect.y1 = y1;
        rect.x2 = x2;
        rect.y2 = y2;

        rects.push_back(rect);

        //Imprime los rectángulos generados
        //cout << "Rectangulo " << i + 1 << ": " << x1 << ", " << x2 << " " << y1 << ", " << y2 << endl;
    }
    return rects;
}    

// Función que crea rectángulos aleatorios para las Q consultas 
vector<Rectangle> createRandomRectanglesQ(int n){ 
    vector<Rectangle> rects; 
    random_device rd; 
    mt19937 gen(rd()); 
 
    //Valores aleatorios entre 0 y 500mil 
    uniform_int_distribution<> distr1(0, 400000); 
    //El tamaño de los lados de los rectángulos es aleatorio entre 0 y 100mil 
    uniform_int_distribution<> distr2(0, 100000); 
 
    for (int i=0; i<n; i++){ // 100Q 
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
        Rectangle rect; 
        rect.x1 = x1; 
        rect.y1 = y1; 
        rect.x2 = x2; 
        rect.y2 = y2; 
 
        rects.push_back(rect); 
    } 
    return rects; 
}
int main(){
    //Crea rectángulos y hace hojas desde 2^10 hasta 2^25
    //Crea rectángulos para las Q consultas
    vector<Rectangle> rectsQ = createRandomRectanglesQ(100);
    //Guardamos rectsQ en un archivo
    ofstream file("rectsQ.txt");
    for (int i = 0; i < rectsQ.size(); i++){
        file << rectsQ[i].x1 << " " << rectsQ[i].y1 << " " << rectsQ[i].x2 << " " << rectsQ[i].y2 << endl;
    }
    file.close();
    cout << "Q listo" << endl;

    for (int i = 10; i<=25; i++){
        vector<Rectangle> rects = createRandomRectangles(i);
        RTree rtree1 = nearestX(rects, 1024, i);
        RTree rtree2 = HilbertRTree(rects, 1024, i);
        RTree rtree3 = SortTileRecursive(rects, 1024, i);
    }
    return 0;
}