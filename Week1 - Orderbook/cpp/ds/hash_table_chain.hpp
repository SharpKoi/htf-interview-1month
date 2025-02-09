#include <optional>


const double A = 0.6180339887;


int moduloHashing(int key, int tableSize) {
    return key % tableSize;
}


template<typename T>
class HashTable {
private:
    struct Node {
        int key;
        std::optional<T> value;
        Node* next;
    };
    
    int size;
    int count;
    Node** storage;

    int hash(int key) {
        return moduloHashing(key, size);
    }

    void extend() {
        size *= 2;
        Node** newStorage = new Node*[size]();

        for(Node* header : oldStorage) {
            Node* cursor = header;
            while(cursor){
                Node* next = cursor->next;
                hk = hash(cursor->key);
                
                cursor->next = newStorage[hk];
                newStorage[hk] = cursor;
                
                cursor = next;
            } 
        }

        delete[] storage;
        storage = newStorage;
    }
public:
    HashTable(int size) : size(size), count(0) {
        storage = new Node*[size]();
    }

    ~HashTable() { delete[] storage; }

    std::optional<T> get(int key) {
        int hk = hash(key);
        Node *cursor = storage[hk];
        if(!cursor) {
            return std::nullopt;
        }

        while(cursor) {
            if(cursor->key == key) {
                return cursor->value;
            }
            cursor = cursor->next;
        }

        return std::nullopt;
    }

    void insert(int key, T val) {
        int hk = hash(key);

        Node* newNode = new Node();
        newNode->key = key;
        newNode->value = val;
        newNode->next = NULL;

        Node* cursor = storage[hk];
        if(!cursor) {
            storage[hk] = newNode;
            return;
        }
        
        while(Node* next = cursor->next) {
            if(next->key == key) {
                next->value = val;
                return;
            }
            cursor = next;
        }
        cursor->next = newNode;

        count++;

        load = (double) count / size;
        if(load >= 0.75){
            extend();
        }
    }

    bool remove(int key) {
        int hk = hash(key);
        
        Node* cursor = storage[hk];
        if(!cursor) {
            return false;
        }
        if(cursor->key == key) {
            storage[hk] = cursor->next;
            delete cursor;
            count--;

            return true;
        }

        while(cursor->next) {
            if(cursor->next->key == key) {
                Node* target = cursor->next;
                cursor->next = target->next;
                delete target;
                count--;

                return true;
            }
            cursor = cursor->next;
        }
        
        return false;
    }
};
