#include <optional>


const double A = 0.6180339887;


int moduloHashing(int key, int tableSize) {
    return key % tableSize;
}


template<typename T>
class HashTable {
private:
    struct Entry {
        int key;
        std::optional<T> value;
        bool is_deleted = false;
    };
    
    int size;
    Entry* storage;

    int hash(int key, int attempt = 0) {
        return (moduloHashing(key, size) + attempt) % size;
    }
public:
    HashTable(int size) : size(size) {
        storage = new Entry[size];
        for(int i = 0; i < size; i++) {
            storage[i].value = std::nullopt;
        }
    }

    ~HashTable() { delete[] storage; }

    std::optional<T> get(int key) {
        for(int i = 0; i < size; i++) {
            int k = hash(key, i);
            if(!storage[k].value && (!storage[k].is_deleted || storage[k].key == key)) {
                return std::nullopt;
            }
            if(!storage[k].is_deleted && storage[k].key == key) {
                return storage[k].value;
            }
        }

        return std::nullopt;
    }

    bool insert(int key, T val) {
        for(int i = 0; i < size; i++) {
            int k = hash(key, i);
            if(!storage[k].value) {
                storage[k].key = key;
                storage[k].value = val;
                storage[k].is_deleted = false;
                return true;
            }
        }

        return false;
    }

    bool remove(int key) {
        for(int i = 0; i < size; i++) {
            int k = hash(key, i);
            if(!storage[k].is_deleted && storage[k].key == key) {
                storage[k].is_deleted = true;
                storage[k].value = std::nullopt;
                return true;
            }
        }

        return false;
    }
};
