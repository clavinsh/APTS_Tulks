#include <stdio.h>
#include <string.h>
#include <fstream>

const int MAX_CSTRING_SIZE = 21;
const float LOAD_FACTOR = 0.75;

class Map {
private:
    static const int DEFAULT_CAPACITY = 16;

    struct Node {
        char key[MAX_CSTRING_SIZE];
        char value[MAX_CSTRING_SIZE];
        int count;
        size_t hashValue;
        Node* next;
    };

    Node** buckets;
    int size;
    int capacity;


    // hash function for bucket determination
    // ensures on average constant lookup, insert, delete
    size_t hash(const char* key) {
        
        size_t value = 0;

        // multiplies by 33: value << 5 is 2^5 = 32; 32*value+value = 33*value
        while (*key) {
            value = (value << 5) + value + *key++;
        }

        return value;
    }

    // copies the contents of map into a resized array, if the used space exceeds load factor
    void resize() {
        if (size >= LOAD_FACTOR * capacity) {
            int newCapacity = capacity * 2;
            Node** newBuckets = new Node * [newCapacity];

            memset(newBuckets, 0, newCapacity * sizeof(Node*));

            for (int i = 0; i < capacity; i++) {
                Node* node = buckets[i];
                while (node != nullptr) {
                    Node* next = node->next;
                    int index = node->hashValue % newCapacity;
                    node->next = newBuckets[index];
                    newBuckets[index] = node;
                    node = next;
                }
            }

            delete[] buckets;
            buckets = newBuckets;
            capacity = newCapacity;


        }
    }

public:
    Map() : size(0), capacity(DEFAULT_CAPACITY) {
        buckets = new Node * [capacity];
        memset(buckets, 0, capacity * sizeof(Node*));
    }

    ~Map() {
        for (int i = 0; i < capacity; i++) {
            Node* node = buckets[i];
            while (node != nullptr) {
                Node* next = node->next;
                delete node;
                node = next;
            }
        }
        delete[] buckets;
    }

    void put(const char* key, const char* value) {
        resize();

        size_t hashValue = hash(key);
        int index = hashValue % capacity;

        Node* node = buckets[index];

        while (node != nullptr) {
            if (strcmp(node->key, key) == 0 && strcmp(node->value, value) == 0) {
                node->count++;
                return;
            }

            node = node->next;
        }

        Node* newNode = new Node();

        strncpy(newNode->key, key, MAX_CSTRING_SIZE);
        strncpy(newNode->value, value, MAX_CSTRING_SIZE);

        newNode->count = 1;
        newNode->hashValue = hashValue;
        newNode->next = buckets[index];

        buckets[index] = newNode;
        size++;

    }

    const char* get(const char* key) {
        size_t hashValue = hash(key);
        int index = hashValue % capacity;

        Node* node = buckets[index];

        while (node != nullptr) {
            if (strcmp(node->key, key) == 0 ) {
                return node->value;
            }

            node = node->next;
        }

        return nullptr;
    }
};

int main() {
    FILE* input_file = nullptr;
    FILE* output_file = nullptr;

    if (freopen_s(&input_file, "tulks.in", "r", stdin) != 0) {
        return -1;
    }

    if (freopen_s(&output_file, "tulks.out", "w", stdout) != 0) {
        return -1;
    }

    Map* map = new Map();
    char currentString[21];

    scanf_s("%s", currentString, (unsigned)_countof(currentString));


    // putting the contents of the file into Map
    while (strcmp(currentString, "<--") != 0 || strcmp(currentString, "-->")) {
        char val[21];

        scanf_s("%s", val, (unsigned)_countof(val));

        map->put(currentString, val);

        scanf_s("%s", currentString, (unsigned)_countof(currentString));
    }

    // the direction ('<--' or '-->') signifies whether to following text from file will need to be found/replaced by
    // key or value (essentially should the keys and values should be logically swapped in the map)
    bool direction = currentString[0] == '-' ? false : true;



    return 0;
}

