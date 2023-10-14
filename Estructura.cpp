#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <random>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace std;

const int MAX_CHILDREN = 4;
const int MIN_CHILDREN = 2;

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {} // Constructor
};

struct Rectangle { // Rectangulo con esquinas inferiores izquierda y superior derecha
    // Coordenadas de la esquina inferior izquierda del rectangulo (x1, y1)
    // Coordenadas de la esquina superior derecha del rectangulo (x2, y2)
    double x1, y1, x2, y2;
    Rectangle(double x1 = 0, double y1 = 0, double x2 = 0, double y2 = 0) : x1(x1), y1(y1), x2(x2), y2(y2) {} // Constructor

    double area() const { // Calcula el area del rectangulo
        return (x2 - x1) * (y2 - y1);
    }
    bool contains(const Point& p) const { // Verifica si el punto esta dentro del rectangulo
        return p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2;
    }
    bool intersects(const Rectangle& r) const { // Verifica si el rectangulo se intersecta con otro
        return x1 <= r.x2 && x2 >= r.x1 && y1 <= r.y2 && y2 >= r.y1;
    }
    double distance(const Point& p) const {
        if (contains(p)) return 0;
        double dx = max(x1 - p.x, p.x - x2);
        double dy = max(y1 - p.y, p.y - y2);
        return sqrt(dx * dx + dy * dy);
    }
};

struct Node {
    bool is_leaf;
    Rectangle rect;
    vector<Node*> children;
    vector<Point> points;
    Node(bool is_leaf = false) : is_leaf(is_leaf) {}
    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }
    void add_child(Node* child) {
        children.push_back(child);
        rect.x1 = min(rect.x1, child->rect.x1);
        rect.y1 = min(rect.y1, child->rect.y1);
        rect.x2 = max(rect.x2, child->rect.x2);
        rect.y2 = max(rect.y2, child->rect.y2);
    }
    void add_point(const Point& p) {
        points.push_back(p);
        rect.x1 = min(rect.x1, p.x);
        rect.y1 = min(rect.y1, p.y);
        rect.x2 = max(rect.x2, p.x);
        rect.y2 = max(rect.y2, p.y);
    }
    bool is_full() const {
        return children.size() == MAX_CHILDREN;
    }
    bool is_underflow() const {
        return children.size() < MIN_CHILDREN;
    }
};

class RTree {
public:
    RTree() : root(nullptr) {}
    ~RTree() { // Destructor
        delete root;
    }
    void insert(const Point& p) { // Inserta un punto en el RTree   
        if (root == nullptr) {
            root = new Node(true);
            root->add_point(p);
            return;
        }
        Node* leaf = choose_leaf(root, p);
        leaf->add_point(p);
        if (leaf->is_full()) { // Si la hoja esta llena, se divide
            Node* split_node = split_leaf(leaf);
            adjust_tree(leaf, split_node);
        }
    }
    void remove(const Point& p) { // Elimina un punto del RTree
        if (root == nullptr) {
            return;
        }
        Node* leaf = find_leaf(root, p); // Busca la hoja que contiene el punto
        if (leaf == nullptr) {
            return;
        }
        auto it = find(leaf->points.begin(), leaf->points.end(), p); // Elimina el punto de la hoja
        if (it != leaf->points.end()) {
            leaf->points.erase(it);
            condense_tree(leaf);
        }
    }
    vector<Point> search(const Rectangle& rect) const { // Busca todos los puntos dentro de un rectangulo
        vector<Point> result;
        if (root == nullptr) {
            return result;
        }
        search(root, rect, result);
        return result;
    }
private:
    Node* root;
    Node* choose_leaf(Node* node, const Point& p) {
        if (node->is_leaf) {
            return node;
        }
        double min_enlargement = INFINITY;
        Node* best_child = nullptr;
        for (Node* child : node->children) {
            double enlargement = enlarged_area(child->rect, p) - child->rect.area();
            if (enlargement < min_enlargement) {
                min_enlargement = enlargement;
                best_child = child;
            } else if (enlargement == min_enlargement) {
                double area = child->rect.area();
                if (area < best_child->rect.area()) {
                    best_child = child;
                }
            }
        }
        return choose_leaf(best_child, p);
    }
    double enlarged_area(const Rectangle& rect, const Point& p) const {
        double x1 = min(rect.x1, p.x);
        double y1 = min(rect.y1, p.y);
        double x2 = max(rect.x2, p.x);
        double y2 = max(rect.y2, p.y);
        return (x2 - x1) * (y2 - y1);
    }
    Node* split_leaf(Node* leaf) {
        Node* new_leaf = new Node(true);
        int n = leaf->points.size();
        for (int i = n / 2; i < n; i++) {
            new_leaf->add_point(leaf->points[i]);
        }
        leaf->points.resize(n / 2);
        if (leaf == root) {
            root = new Node(false);
            root->add_child(leaf);
        }
        root->add_child(new_leaf);
        return new_leaf;
    }
    Node* split_node(Node* node) {
        Node* new_node = new Node(false);
        int n = node->children.size();
        for (int i = n / 2; i < n; i++) {
            new_node->add_child(node->children[i]);
        }
        node->children.resize(n / 2);
        if (node == root) {
            root = new Node(false);
            root->add_child(node);
        }
        root->add_child(new_node);
        return new_node;
    }
    void adjust_tree(Node* node, Node* split_node) {
        if (node == root) {
            root = new Node(false);
            root->add_child(node);
        }
        root->add_child(split_node);
        if (node->is_underflow()) {
            Node* parent = find_parent(root, node);
            if (parent == nullptr) {
                return;
            }
            Node* sibling = find_sibling(parent, node);
            if (sibling == nullptr) {
                return;
            }
            if (sibling->children.size() + node->children.size() <= MAX_CHILDREN) {
                merge_nodes(parent, sibling, node);
            } else {
                Node* new_node = redistribute_nodes(parent, sibling, node);
                adjust_tree(parent, new_node);
            }
        }
    }
    Node* find_leaf(Node* node, const Point& p) const {
        if (node->is_leaf) {
            for (Node* child : node->children) {
                if (child->rect.contains(p)) {
                    return child;
                }
            }
            return nullptr;
        }
        for (Node* child : node->children) {
            if (child->rect.contains(p)) {
                return find_leaf(child, p);
            }
        }
        return nullptr;
    }
    void search(Node* node, const Rectangle& rect, vector<Point>& result) const {
        if (node->is_leaf) {
            for (const Point& p : node->points) {
                if (rect.contains(p)) {
                    result.push_back(p);
                }
            }
        } else {
            for (Node* child : node->children) {
                if (child->rect.intersects(rect)) {
                    search(child, rect, result);
                }
            }
        }
    }
    Node* find_parent(Node* node, Node* child) const {
        if (node->is_leaf) {
            return nullptr;
        }
        for (Node* n : node->children) {
            if (n == child) {
                return node;
            }
            Node* parent = find_parent(n, child);
            if (parent != nullptr) {
                return parent;
            }
        }
        return nullptr;
    }
    Node* find_sibling(Node* parent, Node* node) const {
        for (Node* sibling : parent->children) {
            if (sibling != node) {
                return sibling;
            }
        }
        return nullptr;
    }
    void merge_nodes(Node* parent, Node* sibling, Node* node) {
        sibling->rect.x1 = min(sibling->rect.x1, node->rect.x1);
        sibling->rect.y1 = min(sibling->rect.y1, node->rect.y1);
        sibling->rect.x2 = max(sibling->rect.x2, node->rect.x2);
        sibling->rect.y2 = max(sibling->rect.y2, node->rect.y2);
        for (const Point& p : node->points) {
            sibling->add_point(p);
        }
        for (Node* child : node->children) {
            sibling->add_child(child);
        }
        auto it = find(parent->children.begin(), parent->children.end(), node);
        parent->children.erase(it);
        delete node;
    }
    Node* redistribute_nodes(Node* parent, Node* sibling, Node* node) {
        if (parent->children[0]->is_leaf) {
            int n = sibling->children.size() + node->children.size();
            int k = (n + 1) / 2;
            if (sibling->children.size() < k) {
                for (int i = 0; i < k - sibling->children.size(); i++) {
                    sibling->add_point(node->points[i]);
                }
                node->points.erase(node->points.begin(), node->points.begin() + k - sibling->children.size());
                for (int i = 0; i < k - sibling->children.size(); i++) {
                    sibling->add_child(node->children[i]);
                }
                node->children.erase(node->children.begin(), node->children.begin() + k - sibling->children.size());
            } else if (node->children.size() < k) {
                int m = sibling->children.size();
                for (int i = k - node->children.size(); i < m; i++) {
                    node->add_point(sibling->points[i]);
                }
                sibling->points.erase(sibling->points.begin() + k - node->children.size(), sibling->points.end());
                for (int i = k - node->children.size(); i < m; i++) {
                    node->add_child(sibling->children[i]);
                }
                sibling->children.erase(sibling->children.begin() + k - node->children.size(), sibling->children.end());
            } else {
                int m = sibling->children.size();
                for (int i = 0; i < m - k; i++) {
                    node->add_point(sibling->points.back());
                    sibling->points.pop_back();
                }
                for (int i = 0; i < m - k; i++) {
                    node->add_child(sibling->children.back());
                    sibling->children.pop_back();
                }
            }
        } else {
            int n = sibling->children.size() + node->children.size();
            int k = (n + 1) / 2;
            if (sibling->children.size() < k) {
                for (int i = 0; i < k - sibling->children.size(); i++) {
                    sibling->add_child(node->children[i]);
                }
                node->children.erase(node->children.begin(), node->children.begin() + k - sibling->children.size());
                node->children.front()->rect = node->rect;
                for (int i = 0; i < k - sibling->children.size(); i++) {
                    sibling->rect.x1 = min(sibling->rect.x1, node->children[i]->rect.x1);
                    sibling->rect.y1 = min(sibling->rect.y1, node->children[i]->rect.y1);
                    sibling->rect.x2 = max(sibling->rect.x2, node->children[i]->rect.x2);
                    sibling->rect.y2 = max(sibling->rect.y2, node->children[i]->rect.y2);
                }
            } else if (node->children.size() < k) {
                int m = sibling->children.size();
                for (int i = k - node->children.size(); i < m; i++) {
                    node->add_child(sibling->children[i]);
                }
                sibling->children.erase(sibling->children.begin() + k - node->children.size(), sibling->children.end());
                node->children.front()->rect = node->rect;
                for (int i = k - node->children.size(); i < m; i++) {
                    node->rect.x1 = min(node->rect.x1, sibling->children[i]->rect.x1);
                    node->rect.y1 = min(node->rect.y1, sibling->children[i]->rect.y1);
                    node->rect.x2 = max(node->rect.x2, sibling->children[i]->rect.x2);
                    node->rect.y2 = max(node->rect.y2, sibling->children[i]->rect.y2);
                }
            } else {
                int m = sibling->children.size();
                for (int i = 0; i < m - k; i++) {
                    node->add_child(sibling->children.back());
                    sibling->children.pop_back();
                }
                node->children.front()->rect = node->rect;
                for (int i = 0; i < m - k; i++) {
                    node->rect.x1 = min(node->rect.x1, node->children.back()->rect.x1);
                    node->rect.y1 = min(node->rect.y1, node->children.back()->rect.y1);
                    node->rect.x2 = max(node->rect.x2, node->children.back()->rect.x2);
                    node->rect.y2 = max(node->rect.y2, node->children.back()->rect.y2);
                    node->children.pop_back();
                }
            }
        }
        return node;
    }
    void condense_tree(Node* leaf) {
        Node* parent = find_parent(root, leaf);
        if (parent == nullptr) {
            return;
        }
        if (leaf->is_underflow()) {
            Node* sibling = find_sibling(parent, leaf);
            if (sibling == nullptr) {
                return;
            }
            if (sibling->children.size() + leaf->children.size() <= MAX_CHILDREN) {
                merge_nodes(parent, sibling, leaf);
                condense_tree(parent);
            } else {
                Node* new_node = redistribute_nodes(parent, sibling, leaf);
                condense_tree(new_node);
            }
        } else {
            for (Node* child : parent->children) {
                if (child != leaf) {
                    child->rect.x1 = min(child->rect.x1, leaf->rect.x1);
                    child->rect.y1 = min(child->rect.y1, leaf->rect.y1);
                    child->rect.x2 = max(child->rect.x2, leaf->rect.x2);
                    child->rect.y2 = max(child->rect.y2, leaf->rect.y2);
                }
            }
        }
    }
};

//Función para generar números aleatorios
int random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

//Función para generar números aleatorios con distribución normal
int random_normal(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(min, max);
    return d(gen);
}

//Función para generar un par de rectángulos aleatorios
vector<Rectangle> random_rectangles(int min, int max) {
    vector<Rectangle> rects;
    for (int i = 0; i < 2; i++) {
        Rectangle rect;
        rect.x1 = random(min, max);
        rect.y1 = random(min, max);
        rect.x2 = random(rect.x1, max);
        rect.y2 = random(rect.y1, max);
        rects.push_back(rect);
    }
    return rects;
}

//Función para generar un par de rectángulos aleatorios con distribución normal
vector<Rectangle> random_rectangles_normal(int min, int max) {
    vector<Rectangle> rects;
    for (int i = 0; i < 2; i++) {
        Rectangle rect;
        rect.x1 = random_normal(min, max);
        rect.y1 = random_normal(min, max);
        rect.x2 = random_normal(rect.x1, max);
        rect.y2 = random_normal(rect.y1, max);
        rects.push_back(rect);
    }
    return rects;
}

//Main para generar y graficar los rectángulos aleatorios
int main() {
    //Generación de los rectángulos aleatorios
    vector<Rectangle> rects = random_rectangles(0, 100);
    //Impresión de los rectángulos aleatorios
    cout << "Rectángulos aleatorios: " << endl;
    for (int i = 0; i < rects.size(); i++) {
        cout << "Rectángulo " << i << ": " << endl;
        cout << "x1: " << rects[i].x1 << endl;
        cout << "y1: " << rects[i].y1 << endl;
        cout << "x2: " << rects[i].x2 << endl;
        cout << "y2: " << rects[i].y2 << endl;
    }
    //Generación de los rectángulos aleatorios con distribución normal
    vector<Rectangle> rects_normal = random_rectangles_normal(0, 100);
    //Impresión de los rectángulos aleatorios con distribución normal
    cout << "Rectángulos aleatorios con distribución normal: " << endl;
    for (int i = 0; i < rects_normal.size(); i++) {
        cout << "Rectángulo " << i << ": " << endl;
        cout << "x1: " << rects_normal[i].x1 << endl;
        cout << "y1: " << rects_normal[i].y1 << endl;
        cout << "x2: " << rects_normal[i].x2 << endl;
        cout << "y2: " << rects_normal[i].y2 << endl;
    }
    //Generación de los rectángulos aleatorios con distribución normal
    vector<Rectangle> rects_normal2 = random_rectangles_normal(0, 100);
    //Impresión de los rectángulos aleatorios con distribución normal
    cout << "Rectángulos aleatorios con distribución normal: " << endl;
    for (int i = 0; i < rects_normal2.size(); i++) {
        cout << "Rectángulo " << i << ": " << endl;
        cout << "x1: " << rects_normal2[i].x1 << endl;
        cout << "y1: " << rects_normal2[i].y1 << endl;
        cout << "x2: " << rects_normal2[i].x2 << endl;
        cout << "y2: " << rects_normal2[i].y2 << endl;
    }
    return 0;
}