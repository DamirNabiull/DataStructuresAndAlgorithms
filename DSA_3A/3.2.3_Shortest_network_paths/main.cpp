// Damir Nabiullin B20-03
// Task: 3.2.3 Shortest paths
// Compiler: GNU G++ 14 6.4.0

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <deque>

using namespace std;

// Entry - type of data for PQ
template <typename K, typename V>
class Entry{
private:
    K* key_v;
    V value_v;
public:
    // Constructor of Entry
    Entry(K* key_v, V value_v) {
        this->key_v = key_v;
        this-> value_v = value_v;
    }

    K key() {
        return *(this->key_v);
    }

    V value() {
        return this->value_v;
    }
};

template <typename K, typename V>
class minBinaryHeap{
private:
    vector<Entry<K, V>> heap;

    /**
     * smallestChild
     * @param i
     * @return index of smallest child
     * Time complexity - O(1)
     */
    int smallestChild(int i) {
        if (this->heap.size() == 1) {
            return 0;
        }
        if (2*i + 1 < this->heap.size()) {
            if (this->heap[2*i+1].key() < this->heap[2*i+2].key()) {
                return (2*i+1);
            } else {
                return (2*i+2);
            }
        } else if (2*i + 1 == this->heap.size()) {
            return (2*i + 1);
        } else {
            return 0;
        }
    }

public:
    minBinaryHeap() {}

    /**
     * insert
     * @param key_v
     * @param value_v
     * Time complexity - O(log(n))
     */
    void insert(K* key_v, V value_v) {
        Entry<K, V> e(key_v, value_v);
        this->heap.push_back(e);
        int i = this->heap.size() - 1;
        while (i > 0 && this->heap[(i-1)/2].key() > *key_v) {
            swap(this->heap[i], this->heap[(i-1)/2]);
            i = (i-1)/2;
        }
    }

    /**
     * pop
     * @return value with minimum key
     * Time complexity - O(log(n))
     */
    V pop() {
        V toReturn = this->heap[0].value();
        this->heap[0] = this->heap[this->heap.size()-1];
        this->heap.pop_back();
        int i = 0;
        int child = smallestChild(i);
        while (child != 0 && this->heap[child].key() < this->heap[i].key()) {
            swap(this->heap[child], this->heap[i]);
            i = child;
            child = smallestChild(i);
        }
        return toReturn;
    }

    /**
     * size
     * @return size of PQ
     * Time complexity - O(1)
     */
    int size(){
        return this->heap.size();
    }
};

// Enum of colors for dfs
enum COLOR {
    WHITE,
    GRAY,
    BLACK
};

// Struct of data for third task
struct weightAndBandwidth {
    int weight = 1e8;
    int bandwidth = 1e8;

    bool operator<(const weightAndBandwidth &rhs) const {
        if (weight < rhs.weight)
            return true;
        if (rhs.weight < weight)
            return false;
        return bandwidth < rhs.bandwidth;
    }

    bool operator>(const weightAndBandwidth &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const weightAndBandwidth &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const weightAndBandwidth &rhs) const {
        return !(*this < rhs);
    }

    bool operator==(const weightAndBandwidth &rhs) const {
        return weight == rhs.weight &&
               bandwidth == rhs.bandwidth;
    }

    bool operator!=(const weightAndBandwidth &rhs) const {
        return !(rhs == *this);
    }
};

weightAndBandwidth operator+(const weightAndBandwidth& a, const weightAndBandwidth& b) {
    weightAndBandwidth toReturn;
    toReturn.weight = (a.weight + b.weight)%100000000;
    toReturn.bandwidth = min(a.bandwidth, b.bandwidth);
    return toReturn;
}

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

    /**
     * dijkstra
     * @param start
     * @param end
     * @param initialWeight
     * @return weight of distance to final vertex
     * Time complexity - O(E*log(V))
     */
    W dijkstra(Vertex<V, W>* start, Vertex<V, W>* end, W initialWeight) {
        // Initialization - O(V)
        for (auto i : this->mapVertices) {
            Vertex<V, W>* u = i.second;
            if (u != nullptr) {
                u->parent = nullptr;
                u->distance = W();
                if (u == start) {
                    u->distance = initialWeight;
                }
            }
        }

        // Creation of PQ - O(1)
        minBinaryHeap<W, Vertex<V, W>*> Q;
        Q.insert(&start->distance, start);

        // Emptying and Updating the PQ - O(E*log(V))
        while (Q.size() > 0) {
            Vertex<V, W>* u = Q.pop();
            for (int i = 0; i < count; i++) {
                Edge<V, W> *edge = this->matrix[u->getIndex()][i];
                if (edge != nullptr) {
                    Vertex<V, W>* v = edge->getTo();
                    if (v != nullptr) {
                        if (v->distance > u->distance + edge->getWeight()) {
                            v->distance = u->distance + edge->getWeight();
                            v->parent = u;
                            Q.insert(&v->distance, v);
                        }
                    }
                }
            }
        }

        return end->distance;
    }
};



int main() {
    AdjacencyMatrixGraph<int, weightAndBandwidth> g;

    int N, M, W, from, to, len, weight, start, end;
    vector<pair<pair<int, int>, pair<int, int>>> arr;
    pair<int, int> p1, p2;
    pair<pair<int, int>, pair<int, int>> temp;

    cin >> N >> M;

    for (int i = 0; i < N; i++) {
        g.addVertex(i+1);
    }

    for (int i = 0; i < M; i++) {
        cin >> from >> to >> len >> weight;
        p1.first = from;
        p1.second = to;
        p2.first = len;
        p2.second = weight;
        temp.first = p1;
        temp.second = p2;
        arr.push_back(temp);
    }

    cin >> start >> end >> W;

    for (int i = 0; i < M; i++) {
        temp = arr[i];
        p1 = temp.first;
        p2 = temp.second;
        if (p2.second >= W) {
            Vertex<int, weightAndBandwidth>* a = g.findVertex(p1.first);
            Vertex<int, weightAndBandwidth>* b = g.findVertex(p1.second);
            weightAndBandwidth temp_weight;
            temp_weight.weight = p2.first;
            temp_weight.bandwidth = p2.second;
            g.addEdge(a, b, temp_weight);
        }
    }

    Vertex<int, weightAndBandwidth>* a = g.findVertex(start);
    Vertex<int, weightAndBandwidth>* b = g.findVertex(end);
    weightAndBandwidth ans = g.dijkstra(a, b, {0, 10000000});
    weightAndBandwidth maximum = weightAndBandwidth();
    if (ans == maximum) {
        cout << "IMPOSSIBLE\n";
    } else {
        vector<int> path;
        while (b != nullptr) {
            path.push_back(b->getValue());
            b = b->parent;
        }
        cout << path.size() << " " << ans.weight << " " << ans.bandwidth << "\n";
        for (int i = path.size() - 1; i >= 0; i--) {
            cout << path[i] << " ";
        }
    }
}
