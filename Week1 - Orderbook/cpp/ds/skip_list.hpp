#include <iostream>
#include <optional>

template<typename T>
class SkipList {
private:
    struct Node {
        double key;
        std::optional<T> value;
        int level;
        Node** next;

        Node(double key, std::optional<T> value=std::nullopt, int level=0)
         : key(key), value(value), level(level) {
            if(level > 0) {
                next = new Node*[level+1]();  // store the next nodes of every levels.
            }
        }

        ~Node() {
            delete[] next;
        }
    };

    int level;
    int maxLevel;
    Node* header;
    
public:
    SkipList(int maxLevel) : maxLevel(maxLevel), level(0) {
        header = new Node(-1, std::nullopt, maxLevel);
    }

    ~SkipList() {
        delete header;
    }

    void insert(double key, T value) {

    }

    void remove(double key) {

    }

    void get(double key) {

    }
};
