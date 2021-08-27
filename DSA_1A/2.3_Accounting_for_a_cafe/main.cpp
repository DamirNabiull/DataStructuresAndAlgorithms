// Damir Nabiullin B20-03
// Task: 2.3 Accounting for a café
// Compiler: GNU G++ 14 6.4.0
// CodeForces submission number - 107968311

#include <iostream>
#include <iomanip>

using namespace std;

// Class ArrayList
template <typename T>
class ArrayList {
private:
    int count, len; // Count of elements in list and length or Array
    T *array; // Array of elements

    /** Function doubling: double our array
     * TIME COMPLEXITY - O(n) : (worst case)
     */
    void doubling() {
        T *temp = new T[len*2];
        for (int i = 0; i < len; ++i) {
            temp[i] = array[i];
        }
        array = temp;
        len = len * 2;
    }

public:
    // Constructor of ArrayList
    ArrayList() {
        array = new T[1];
        count = 0;
        len = 1;
    }

    /** Function addLast: add element at first free position
     * TIME COMPLEXITY - O(n) : (worst case) *doubling
     */
    void addLast(T value) {
        if (count == len) {
            doubling();
        }
        array[count] = value;
        count++;
    }

    /** Function get: return element with given index
     * TIME COMPLEXITY - O(1) : (worst case)
     */
    T get(int index) {
        if (index >= 0 && index < count) {
            return array[index];
        } else {
            return array[count-1];
        }
    }

    /** Function set: put element in given index
     * TIME COMPLEXITY - O(1) : (worst case)
     */
    void set(int index, T value) {
        if (index >= 0 && index < count) {
            array[index] = value;
        }
    }

    /** Function add: add element on given position
     * TIME COMPLEXITY - O(n) : (worst case) *doubling
     */
    void add(int index, T value){
        if (count == len) {
            doubling();
        }
        T *temp = new T[len];
        for (int i = 0; i < index; ++i) {
            temp[i] = array[i];
        }
        temp[index] = value;
        for (int i = index+1; i < len; ++i) {
            temp[i] = array[i-1];
        }
        array = temp;
        count++;
    }

    /** Function remove: remove element with given position
     * TIME COMPLEXITY - O(n) : (worst case)
     */
    void remove(int index){
        if (!isEmpty()) {
            T *temp = new T[len];
            for (int i = 0; i < index; ++i) {
                temp[i] = array[i];
            }
            for (int i = index+1; i < len; ++i) {
                temp[i-1] = array[i];
            }
            array = temp;
        }
        count--;
    }

    /** Function addFirst: add element at the first position
     * TIME COMPLEXITY - O(n) : (worst case)
     */
    void addFirst(T value){
        add(0, value);
    }

    /** Function size: return count of elements
     * TIME COMPLEXITY - O(1) : (worst case)
     */
    int size() {
        return count;
    }

    /** Function isEmpty: return is ArrayList empty
     * TIME COMPLEXITY - O(1) : (worst case)
     */
    bool isEmpty() {
        return count == 0;
    }
};

// Abstract class Map
template <typename T, typename E>
class Map {
public:
    virtual void put(T key, E value) = 0; // add a new item to the List
    virtual E get(T key) = 0; // return the i-th least element
    virtual void remove(T key) = 0; // remove i-th least element from the list
    virtual ArrayList<T> keySet() = 0; //return an iterable collection of the keys in map
    virtual ArrayList<E> valuesSet() = 0; // return an iterator of the values in map
    virtual int size() = 0; // return the size of the list
    virtual bool isEmpty() = 0; // return whether the list is empty
};

// Class HashTable - implementation of Map
template <typename T, typename E>
class HashTable : public Map<T, E> {
private:
    ArrayList<T> keys; // ArrayList of keys
    ArrayList<pair<T, E>> *values[15139] = {nullptr}; // Array of pointers to ArrayLists, that store pair of key and value


    // Function HashCode: return hashed index
    int HashCode (string key) {
        string &s = key;
        int hash = 0, t;
        for(int i = 0; i < s.size(); i++){
            t = s[i];
            hash += (t * ((i+1)%43))%15139;
        }
        return (3*hash + 7)%15139;
    }

public:
    // Constructor of HasTable
    HashTable(){}

    /** Function put: put element to given key (Using Separate Chaining)
     */
    void put(T key, E value) override {
        int hash = HashCode(key);
        if (values[hash] == nullptr) {
            auto *temp = new ArrayList<pair<T, E>>;
            pair<T, E> p;
            p.first = key;
            p.second = value;
            temp->addLast(p);
            keys.addLast(key);
            values[hash] = temp;
        } else {
            ArrayList<pair<T, E>> *temp = values[hash];
            bool in = false;
            for (int i = 0; i < temp->size(); i++) {
                pair<T, E> p = temp->get(i);
                if (p.first == key) {
                    in = true;
                    p.second = value;
                    temp->set(i, p);
                    break;
                }
            }
            if (!in) {
                pair<T, E> t_p;
                t_p.first = key;
                t_p.second = value;
                temp->addLast(t_p);
                keys.addLast(key);
            }
        }
    }

    /** Function get: return element with given key (Using Separate Chaining)
     */
    E get(T key) override {
        int hash = HashCode(key);
        if (values[hash] == nullptr) {
            E *ans = new E;
            return *ans;
        } else {
            ArrayList<pair<T, E>> *temp = values[hash];
            pair<T, E> p;
            for (int i = 0; i < temp->size(); i++) {
                p = temp->get(i);
                if (p.first == key) {
                    return p.second;
                }
            }
            E *ans = new E;
            return *ans;
        }
    }

    /** Function remove: remove element with given key and key itself (Using Separate Chaining)
     */
    void remove(T key) override {
        int hash = HashCode(key);
        if (values[hash] != nullptr) {
            ArrayList<pair<T, E>> *temp = values[HashCode(key)];
            pair<T, E> p;
            for (int i = 0; i < temp->size(); i++) {
                p = temp->get(i);
                if (p.first == key) {
                    temp->remove(i);
                    break;
                }
            }
            for (int i = 0; i < keys.size(); ++i) {
                if (keys.get(i) == key) {
                    keys.remove(i);
                    break;
                }
            }
        }
    }

    /** Function keySet: return ArrayList with keys
     * TIME COMPLEXITY - O(1) : (worst case)
     */
    ArrayList<T> keySet() override {
        return keys;
    }

    /** Function valueSet: return ArrayList with elements
     */
    ArrayList<E> valuesSet() override {
        ArrayList<E> ans;
        for (int i = 0; i < keys.size(); ++i) {
            ans.addLast(get(keys.get(i)));
        }
        return ans;
    }

    /** Function size: return count of elements in HashTable
     * TIME COMPLEXITY - O(1) : (worst case)
     */
    int size() override {
        return keys.size();
    }

    /** Function isEmpty: say is HashTable empty
     * TIME COMPLEXITY - O(1) : (worst case)
     */
    bool isEmpty() override {
        return keys.size()==0;
    }
};

// Struct order with sum of elements with given date and ArrayList of different ID with given date
struct order{
    double sum = 0;
    ArrayList<string> ID;
};

// Function read: read input values
void read(string &date, string &id, double &cost, string &temp){
    cin >> date;
    cin >> temp;
    cin >> id;
    cin >> temp;
    temp.replace(0, 1, "");
    cost = stod(temp);
    getline(cin, temp);
}

int main() {
    HashTable<string, order> map;
    int n;
    string date, id, temp;
    double cost;
    bool in = false;
    order item;
    cin >> n; // read number of receipts
    for (int i = 0; i < n; ++i) {
        read(date, id, cost, temp); // read values
        item = map.get(date); // get item from HashTable (Empty if we haven't given key in HashTable)
        for (int j = 0; j < item.ID.size(); ++j) { // Check all ID
            if (item.ID.get(j) == id) { // If we have this ID, we don't add it
                in = true;
                break;
            }
        }
        if(!in){ // If we haven't this ID, we add it
            item.ID.addLast(id);
        }
        in = false;
        item.sum += cost; // Update sum for given date
        map.put(date, item); // Put element to HashTable for given date
    }

    ArrayList<string> keys = map.keySet(); // Get all dates
    for (int i = 0; i < keys.size(); i++) { // Go through all dates
        date = keys.get(i);
        item = map.get(date); // Get value for given date
        cout << date << " " << "$" << fixed << setprecision(2) << item.sum << " " << item.ID.size() << '\n'; // Output answer
    }
}
