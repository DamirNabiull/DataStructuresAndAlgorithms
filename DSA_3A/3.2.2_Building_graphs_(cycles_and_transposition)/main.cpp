// Damir Nabiullin B20-03
// Task: 3.2.2 Cycle detection and transposition
// Compiler: GNU G++ 14 6.4.0

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <deque>

using namespace std;

// Enum of colors for dfs
enum COLOR {
    WHITE,
    GRAY,
    BLACK
};

// Class of vertex
template <typename V, typename W>
class Vertex {
private:
    V value;
    int index;
public:

    COLOR color;
    Vertex<V, W>* parent;
    W distance;

    // Constructor of vertex
    Vertex(V value, int index){
        this->value = value;
        this->index = index;
    }

    // getValue - return value of vertex
    V getValue(){
        return this->value;
    }

    // getIndex - return index of vertex in matrix
    int getIndex(){
        return this->index;
    }
};

// Class of edge
template <typename V, typename W>
class Edge {
private:
    W weight;
    Vertex<V, W>* from;
    Vertex<V, W>* to;
public:

    // Constructor of edge
    Edge(Vertex<V, W>* from, Vertex<V, W>* to, W weight){
        this->from = from;
        this->to = to;
        this->weight = weight;
    }

    // getWeight - return weight of edge
    W getWeight(){
        return this->weight;
    }

    // getFrom - return pointer to vertex "from"
    Vertex<V, W>* getFrom(){
        return this->from;
    }

    // getTo - return pointer to vertex "to"
    Vertex<V, W>* getTo(){
        return this->to;
    }

    // transpose - reverse edge (swap to and from)
    void transpose(){
        swap(this->from, this->to);
    }
};

// Abstract class GraphADT
template <typename V, typename W>
class GraphADT {
public:
    virtual Vertex<V, W>* addVertex(V value) = 0; // add a new vertex with given value - O(V)
    virtual void removeVertex(Vertex<V, W>* v) = 0; // remove vertex from graph - O(V)
    virtual Edge<V, W>* addEdge(Vertex<V, W>* from, Vertex<V, W>* to, W weight) = 0; // add a new edge to graph - O(1)
    virtual void removeEdge(Edge<V, W>* e) = 0; // remove edge from graph - O(1)
    virtual vector<Edge<V, W>*> edgesFrom(Vertex<V, W>* v) = 0; // returns vector of edges from given vertex - O(V)
    virtual vector<Edge<V, W>*> edgesTo(Vertex<V, W>* v) = 0; // returns vector of edges to given vertex - O(V)
    virtual Vertex<V, W>* findVertex(V value) = 0; // return pointer to vertex with given value - O(1)
    virtual Edge<V, W>* findEdge(V from_value, V to_value) = 0; // return pointer to edge with vertices with given values - O(1)
    virtual bool hasEdge(Vertex<V, W>* v, Vertex<V, W>* u) = 0; // return true if there exist edge between given vertices, false otherwise - O(1)
};

// Class AdjacencyMatrixGraph - implementation of GraphADT
template <typename V, typename W>
class AdjacencyMatrixGraph : public GraphADT<V, W>{
private:
    unordered_map<V, Vertex<V, W>*> mapVertices;
    int count;
    vector<vector<Edge<V, W>*>> matrix;
    queue<int> emptyPositions;

    /**
     * DFS - implementation of DFS algorithm (to find cycles)
     * @return true if there is no cycles, false otherwise
     * Time complexity O(|V+E|)
     */
    bool DFS() {
        int k = 0;
        for (auto i : this->mapVertices) {
            Vertex<V, W>* u = i.second;
            if (u != nullptr) {
                u->color = WHITE;
                u->parent = nullptr;
            }
        }

        deque<V> path;
        bool t = true;
        for (auto i : this->mapVertices) {
            Vertex<V, W>* u = i.second;
            if (u != nullptr) {
                if (u->color == WHITE) {
                    t = this->visitDFS(u, &path);
                    if (!t) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    /**
     * visitDFS - visit all unvisited vertices
     * @param u
     * @param path
     * @return true if there is no cycles, false otherwise
     */
    bool visitDFS(Vertex<V, W>* u, deque<V>* path) {
        path->push_back(u->getValue());
        u->color = GRAY;

        for (int i = 0; i < this->count; i++) {
            Edge<V, W>* e = this->matrix[u->getIndex()][i];
            if (e != nullptr) {
                Vertex<V, W>* v = e->getTo();
                bool t = true;
                if (v->color == WHITE) {
                    v->parent = u;
                    t = this->visitDFS(v, path);
                    if (!t) {
                        return false;
                    }
                }
                else if (v->color == GRAY) {
                    W sum = 0;
                    int ind = 0;
                    while (path->at(ind) != v->getValue()) {
                        ind++;
                    }
                    for (int j = ind+1; j < path->size(); j++) {
                        Edge<V, W>* temp = this->findEdge(path->at(j-1), path->at(j));
                        sum += temp->getWeight();
                    }
                    sum = sum + this->findEdge(u->getValue(), v->getValue())->getWeight();
                    cout << sum << " ";
                    for (int j = ind; j < path->size(); j++) {
                        cout << path->at(j) << " ";
                    }
                    cout << "\n";
                    return false;
                }
            }
        }
        u->color = BLACK;
        path->pop_back();
        return true;
    }

public:

    AdjacencyMatrixGraph(){
        for (int i = 0; i < 1000; i++) {
            vector<Edge<V, W>*> temp(1000, nullptr);
            matrix.push_back(temp);
        }
        this->count = 0;
    }


    /**
     * add a new vertex with given value
     * @param value
     * @return pointer to vertex
     * Time complexity - O(V)
     */
    Vertex<V, W>* addVertex(V value) override {
        if (mapVertices[value] != nullptr) {
            return nullptr;
        }
        Vertex<V, W>* v;
        if (emptyPositions.empty()) {
            if (count == matrix.size()) {
                for (int i = 0; i < count; i++) {
                    matrix[i].push_back(nullptr);
                }
                vector<Edge<V, W>*> temp(count+1, nullptr);
                matrix.push_back(temp);
            }
            v = new Vertex<V, W>(value, this->count);
            this->count++;
        } else {
            v = new Vertex<V, W>(value, this->emptyPositions.front());
            this->emptyPositions.pop();
        }
        this->mapVertices[value] = v;
        return v;
    }

    /**
     * removeVertex
     * @param v
     * Time complexity - O(V)
     */
    void removeVertex(Vertex<V, W>* v) override {
        if (v != nullptr) {
            for (int i = 0; i < count; i++) {
                if (matrix[v->getIndex()][i] != nullptr) {
                    this->removeEdge(matrix[v->getIndex()][i]);
                }
                if (matrix[i][v->getIndex()] != nullptr) {
                    this->removeEdge(matrix[i][v->getIndex()]);
                }
            }

            this->emptyPositions.push(v->getIndex());
            this->mapVertices[v->getValue()] = nullptr;
            delete v;
        }
    }

    /**
     * addEdge
     * @param from
     * @param to
     * @param weight
     * @return pointer to edge
     * Time complexity - O(1)
     */
    Edge<V, W>* addEdge(Vertex<V, W>* from, Vertex<V, W>* to, W weight) override {
        if (from != nullptr && to != nullptr) {
            auto e = new Edge<V, W>(from, to, weight);
            matrix[from->getIndex()][to->getIndex()] = e;
            return e;
        }
        return nullptr;
    }

    /**
     * removeEdge
     * @param e
     * Time complexity - O(1)
     */
    void removeEdge(Edge<V, W>* e) override {
        if (e != nullptr) {
            Vertex<V, W>* from = e->getFrom();
            Vertex<V, W>* to = e->getTo();

            matrix[from->getIndex()][to->getIndex()] = nullptr;
            delete e;
        }
    }

    /**
     * edgesFrom
     * @param v
     * @return vector of pointers to edges
     * Time complexity - O(V)
     */
    vector<Edge<V, W>*> edgesFrom(Vertex<V, W>* v) override {
        vector<Edge<V, W>*> temp;
        for (int i = 0; i < count; i++) {
            if (matrix[v->getIndex()][i] != nullptr) {
                temp.push_back(matrix[v->getIndex()][i]);
            }
        }
        return temp;
    }

    /**
     * edgesTo
     * @param v
     * @return vector of pointers to edges
     * Time complexity - O(V)
     */
    vector<Edge<V, W>*> edgesTo(Vertex<V, W>* v) override {
        vector<Edge<V, W>*> temp;
        for (int i = 0; i < count; i++) {
            if (matrix[i][v->getIndex()] != nullptr) {
                temp.push_back(matrix[i][v->getIndex()]);
            }
        }
        return temp;
    }

    /**
     * findVertex
     * @param value
     * @return pointer to vertex
     * Time complexity - O(1)
     */
    Vertex<V, W>* findVertex(V value) override {
        return this->mapVertices[value];
    }

    /**
     * findEdge
     * @param from_value
     * @param to_value
     * @return pointer to edge
     * Time complexity - O(1)
     */
    Edge<V, W>* findEdge(V from_value, V to_value) override {
        Vertex<V, W>* from = this->mapVertices[from_value];
        Vertex<V, W>* to = this->mapVertices[to_value];
        if (from != nullptr && to != nullptr) {
            if (from->getIndex() < count && to->getIndex() < count) {
                return matrix[from->getIndex()][to->getIndex()];
            }
        }
        Edge<V, W>* temp = nullptr;
        return temp;
    }

    /**
     * hasEdge
     * @param v
     * @param u
     * @return true or false
     * Time complexity - O(1)
     */
    bool hasEdge(Vertex<V, W>* v, Vertex<V, W>* u) override {
        if (matrix[v->getIndex()][u->getIndex()] != nullptr && v != nullptr && u != nullptr) {
            return true;
        }
        return false;
    }

    /**
     * isAcyclic
     * @return true if there is no cycles, false otherwise
     * Time complexity O(|V+E|)
     */
    bool isAcyclic() {
        return DFS();
    }

    /**
     * transpose
     * Time complexity - O(V^2)
     */
    void transpose() {
        for (int i = 0; i < this->count; i++) {
            for (int j = i; j < this->count; j++) {
                Edge<V, W>* a = matrix[i][j];
                if (a != nullptr) {
                    a->transpose();
                }
                Edge<V, W>* b = matrix[j][i];
                if (b != nullptr) {
                    b->transpose();
                }
                swap(matrix[i][j], matrix[j][i]);
            }
        }
    }
};

int main() {
    AdjacencyMatrixGraph<string, int> g;

    string s;
    string name1, name2;
    int weight;

    while (cin >> s) {
        if (s == "ADD_VERTEX") {
            cin >> name1;
            Vertex<string, int>* a = g.addVertex(name1);
        }
        else if (s == "REMOVE_VERTEX") {
            cin >> name1;
            Vertex<string, int>* a = g.findVertex(name1);
            g.removeVertex(a);
        }
        else if (s == "ADD_EDGE") {
            cin >> name1 >> name2 >> weight;
            Vertex<string, int>* a = g.findVertex(name1);
            Vertex<string, int>* b = g.findVertex(name2);
            Edge<string, int>* e = g.addEdge(a, b, weight);
        }
        else if (s == "REMOVE_EDGE") {
            cin >> name1 >> name2;
            Edge<string, int>* e = g.findEdge(name1, name2);
            g.removeEdge(e);
        }
        else if (s == "HAS_EDGE") {
            cin >> name1 >> name2;
            Vertex<string, int>* a = g.findVertex(name1);
            Vertex<string, int>* b = g.findVertex(name2);
            if (g.hasEdge(a, b)) {
                cout << "TRUE\n";
            } else {
                cout << "FALSE\n";
            }
        }
        else if (s == "IS_ACYCLIC") {
            if (g.isAcyclic()) {
                cout << "ACYCLIC\n";
            }
        }
        else if (s == "TRANSPOSE") {
            g.transpose();
        }
    }
}

