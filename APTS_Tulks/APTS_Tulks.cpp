#include <stdio.h>
#include <string.h>
#include <fstream>

const int MAX_CSTRING_SIZE = 21;

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

    size_t hash(const char* key) {
        size_t value = 0;

        while (*key) {
            // multiplies the number by 33 (a prime) and adds the ascii char value
            // << 5 - 2^5 = 32
            // + value = 32*value + value = 33*value
            value = (value << 5) + value + *key++;
        }

        return value;
    }

    // copies the contents of map into a resized array, if the used space exceeds load factor
    void resize() {
        if (size >= 0.75 * capacity) {
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

    int get(const char* key, const char* value) {
        size_t hashValue = hash(key);
        int index = hashValue % capacity;
        Node* node = buckets[index];

        while (node != nullptr) {
            if (strcmp(node->key, key) == 0 && strcmp(node->value, value) == 0) {
                return node->count;
            }

            node = node->next;
        }

        return 0;
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

    // faila 'vārdnīcas' satura ielasīšana iekš Map

    char currentString[21];

    while (!feof(input_file)) {
        scanf_s("%s", currentString, (unsigned)_countof(currentString));





        printf(currentString);
    }


    // tālāk noskaidro virzienu --> vai <--

    // tālāk iet pa vardam no fin caur map un beigās izved arā fout (katru vārdu atsevisķi)

    return 0;
}

