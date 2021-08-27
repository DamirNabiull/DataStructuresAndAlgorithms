// Damir Nabiullin B20-03
// Task: 2.2 Managing Pawn Shop Items
// Compiler: GNU G++ 14 6.4.0
// CodeForces submission number - 107775598

#include <iostream>

using namespace std;

//Struct data to store Shop Items cost and name. (With added operators of equality and relations)
struct data {
    double cost;
    string cost_str;
    string name;

    bool operator<(const data &rhs) const {
        if (cost < rhs.cost)
            return true;
        if (rhs.cost < cost)
            return false;
    }

    bool operator>(const data &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const data &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const data &rhs) const {
        return !(*this < rhs);
    }

    bool operator==(const data &rhs) const {
        return cost == rhs.cost &&
               cost_str == rhs.cost_str &&
               name == rhs.name;
    }

    bool operator!=(const data &rhs) const {
        return !(rhs == *this);
    }
};

// Struct Node for List and SortedList. This structure contain value of Node and pointer to another Node
template <typename T>
struct Node {
    T value;
    Node *next;
};

// Class List (Implemented for function searchRange from SortedList)
template <typename T>
class List {
private:
    int count; // Count of elements in list
    Node<T> *head = new Node<T>; // Pointer to first element in List
    Node<T> *tail = new Node<T>; // Pointer to last element in List
public:
    // Constructor of List
    List() {
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    // Function addLast: put given element at the last position
    // TIME COMPLEXITY - O(1) : (worst case)
    void addLast(T value) {
        auto *temp = new Node<T>;
        temp->value = value;
        temp->next = nullptr;
        if (head == nullptr) { // We check is there any element in list or no
            head = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = temp;
        }
        count++;
    }

    // Function addFirst: put given element at the first position
    // TIME COMPLEXITY - O(1) : (worst case)
    void addFirst(T value) {
        auto *temp = new Node<T>;
        temp->value = value;
        if (head == nullptr) { // We check is there any element in list or no
            head = temp;
            tail = temp;
            temp->next = nullptr;
        } else {
            temp->next = head;
            head = temp;
        }
        count++;
    }

    // Function removeFirst: delete the first element of list
    // TIME COMPLEXITY - O(1) : (worst case)
    void removeFirst() {
        auto *temp = head->next;
        head = nullptr;
        head = temp;
        count--;
    }

    // Function removeLast: delete the last element of list
    // TIME COMPLEXITY - O(n) : (worst case)
    void removeLast() {
        remove(count-1);
    }

    // Function get: returns the element with given index
    // TIME COMPLEXITY - O(n) : (worst case)
    T get(int index) {
        if (!isEmpty() && index >= 0 && index < count) { // Check: is our index in list
            Node<T> *temp;
            temp = head;
            for (int j = 0; j < index; j++) {
                temp = temp->next;
            }
            return temp->value; // return value with given index
        } else {
            T temp_value;
            return temp_value; // return empty element
        }
    }

    // Function set: put to given position given value
    // TIME COMPLEXITY - O(n) : (worst case)
    void set(T value, int index) {
        Node<T> *cur;
        cur = head;
        for (int i = 0; i < index; i++) {
            cur = cur->next;
        }
        cur->value = value;
    }

    // Function add: put element between elements with (index - 1) and (index + 1)
    // TIME COMPLEXITY - O(n) : (worst case)
    void add(T value, int index) {
        if (index == 0) {
            addFirst(value);
        } else {
            auto *temp = new Node<T>;
            Node<T> *cur, *prev;
            temp->value = value;
            cur = head;
            for (int i = 0; i < index; i++) {
                prev = cur;
                cur = cur->next;
            }
            prev->next = temp;
            temp->next = cur;
            count++;
        }
    }

    // Function remove: delete element with given index
    // TIME COMPLEXITY - O(n) : (worst case)
    void remove(int index) {
        if (index == 0) {
            removeFirst();
        } else {
            Node<T> *cur, *prev;
            cur = head;
            for (int i = 0; i < index; i++) {
                prev = cur;
                cur = cur->next;
            }
            prev->next = cur->next;
            cur = nullptr;
            if (index == count - 1) {
                tail = prev;
            }
            count--;
        }
    }

    // Function size: return amount of elements in list
    // TIME COMPLEXITY - O(1) : (worst case)
    int size() {
        return count;
    }

    // Function isEmpty: say is our list empty or no
    // TIME COMPLEXITY - O(1) : (worst case)
    bool isEmpty() {
        return count == 0;
    }

};

// Abstract class SortedList
template <typename T>
class SortedList {
public:
    virtual void add(T item) = 0; // add a new item to the List
    virtual T least() = 0; // return the least element
    virtual T greatest() = 0; // return the greatest element
    virtual T get(int i) = 0; // return the i-th least element
    virtual int indexOf(T item) = 0; // return the index of an element (in a sorted sequence)
    virtual void remove(int i) = 0; // remove i-th least element from the list
    virtual List<T> searchRange(T from, T to) = 0; // find all items between from and to
    virtual int size() = 0; // return the size of the list
    virtual bool isEmpty() = 0; // return whether the list is empty
};

// Class LinkedSortedList - implementation of SortedList
template <typename T>
class LinkedSortedList: public SortedList<T> {
private:
    int count; // Count of elements in list
    Node<T> *head = new Node<T>; // Pointer to the first element in LinkedSortedList
    Node<T> *tail = new Node<T>; // Pointer to the last element in LinkedSortedList

public:
    // Constructor of LinkedSortedList
    LinkedSortedList() {
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    // Function add: put element in LinkedSortedList at sorted position
    // TIME COMPLEXITY - O(n) : (worst case)
    // If no elements in LinkedSortedList, element less than first or more than last - TIME COMPLEXITY = O(1)
    // If element more than first and less than last - TIME COMPLEXITY = O(n) (we check all elements while our element is greater than element in LinkedSortedList)
    void add(T item) override {
        auto *temp = new Node<T>;
        temp->value = item;
        temp->next = nullptr;
        if (head == nullptr) {
            head = temp;
            tail = temp;
            count++;
        } else if (item <= least()) {
            temp->next = head;
            head = temp;
            count++;
        } else if (item >= greatest()) {
            tail->next = temp;
            tail = temp;
            count++;
        } else {
            Node<T> *cur, *prev;
            cur = head;
            while (cur->value < temp->value) {
                prev = cur;
                cur = cur->next;
            }
            prev->next = temp;
            temp->next = cur;
            count++;
        }
    }

    // Function least: return the least element of LinkedSortedList
    // TIME COMPLEXITY - O(1) : (worst case)
    T least() override {
        return head->value;
    }

    // Function greatest: return the greatest element of LinkedSortedList
    // TIME COMPLEXITY - O(1) : (worst case)
    T greatest() override {
        return tail->value;
    }

    // Function get: return element of LinkedSortedList with given index
    // TIME COMPLEXITY - O(n) : (worst case)
    // If LinkedSortedList is empty, return empty element - TIME COMPLEXITY = O(1)
    // If LinkedSortedList is empty, we go through all indexes till given - TIME COMPLEXITY = O(n)
    T get(int i) override {
        if (!isEmpty() && i >= 0 && i < count) {
            Node<T> *temp;
            temp = head;
            for (int j = 0; j < i; j++) {
                temp = temp->next;
            }
            return temp->value;
        } else {
            T temp_value;
            return temp_value;
        }
    }

    // Function indexOf: return index of given element in LinkedSortedList
    // TIME COMPLEXITY - O(n) : (worst case)
    // If LinkedSortedList is empty, return -1 - TIME COMPLEXITY = O(1)
    // If LinkedSortedList is empty, we go through all elements while element is not equal to given- TIME COMPLEXITY = O(n)
    int indexOf(T item) override {
        if (!isEmpty()) {
            Node<T> *temp;
            temp = head;
            int index = 0;
            while  (temp->value != item && index < count-1) {
                temp = temp->next;
                index++;
            }
            if (temp->value == item) {
                return index;
            }
        }
        return -1;
    }

    // Function remove: delete element of LinkedSortedList with given index
    // TIME COMPLEXITY - O(n) : (worst case)
    // If index = 0, remove the first element - TIME COMPLEXITY = O(1)
    // If index less than 0 and more than count of elements - TIME COMPLEXITY = O(1)
    // If count > index > 0, we go through all indexes till given, and remove - TIME COMPLEXITY = O(n)
    void remove(int i) override {
        if (i == 0) {
            auto *temp = head->next;
            head = nullptr;
            head = temp;
            if (i == count - 1) {
                tail = temp;
            }
            count--;
        } else if (i > 0 && i < count) {
            Node<T> *cur, *prev;
            cur = head;
            for (int j = 0; j < i; j++) {
                prev = cur;
                cur = cur->next;
            }
            prev->next = cur->next;
            cur = nullptr;
            if (i == count - 1) {
                tail = prev;
            }
            count--;
        }
    }

    // Function searchRange: search elements of LinkedSortedList with values between or equal values from and to
    // TIME COMPLEXITY - O(n) : (worst case)
    // If LinkedSortedList is empty, return empty element - TIME COMPLEXITY = O(1)
    // If value of to less than value of head, return empty element  - TIME COMPLEXITY = O(1)
    // In other case: We go through all values while value less than value of to and while element is not the last  - TIME COMPLEXITY = O(n)
    List<T> searchRange(T from, T to) override {
        Node<T> *temp;
        List<T> list;
        if (!isEmpty()) {
            temp = head;
            int index = 0;
            while ((temp->value <= to)  && (index < count-1)) {
                if (temp->value >= from) {
                    list.addLast(temp->value);
                }
                temp = temp->next;
                index++;
            }
            if (temp->value >= from && temp->value <= to) {
                list.addLast(temp->value);
            }
        }
        return list;
    }

    // Function size: return count of elements in LinkedSortedList
    // TIME COMPLEXITY - O(1) : (worst case)
    int size() override {
        return count;
    }

    // Function isEmpty: say is our list empty or no
    // TIME COMPLEXITY - O(1) : (worst case)
    bool isEmpty() override {
        return count == 0;
    }
};

// Function read: reads cost and name of items
void read_item(data &item, string &s){
    cin >> s;
    item.cost_str = s;
    s.replace(0, 1, "");
    item.cost = stod(s);
    getline(cin, s);
    s.replace(0, 1, "");
    item.name = s;
}

int main() {
    int n, index;
    string s;
    LinkedSortedList<data> myList;
    data item, from, to;

    cin >> n; // Read number of elements
    for (int i = 0; i < n; i++) {
        cin >> s; // Read command
        if (s == "ADD") {
            read_item(item, s); // Read item
            myList.add(item); // Add item to SortedList
        } else if (s == "REMOVE") {
            read_item(item, s); // Read item
            index = myList.indexOf(item); // Take index of this item
            myList.remove(index); // Remove element from list
        } else {
            cin >> s; // Read cost - from
            from.cost_str = s;
            s.replace(0, 1, "");
            from.cost = stod(s);

            cin >> s; // Read cost - to
            to.cost_str = s;
            s.replace(0, 1, "");
            to.cost = stod(s);

            List<data> ans;
            ans = myList.searchRange(from, to);
            for (int j = 0; j < ans.size(); j++) {
                if (j != 0) {
                    cout << ", ";
                }
                cout << ans.get(j).cost_str << " " << ans.get(j).name;
            }
            cout << '\n';
        }
    }
}
