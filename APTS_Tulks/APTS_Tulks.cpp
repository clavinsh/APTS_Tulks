#include <fstream>

const int MAX_CSTRING_SIZE = 21;
const float LOAD_FACTOR = 1;
const int DEFAULT_CAPACITY = 50001;//16*16*16*16

namespace helpers {
    size_t strlen(const char* str) {
        size_t size = 0;
        while (*str != '\0') {
            ++size;
            ++str;
        }
        return size;
    }

    int strcmp(const char* str1, const char* str2) {
        while (*str1 != '\0' && *str2 != '\0') {
            if (*str1 != *str2) {
                return (*str1 < *str2) ? -1 : 1;
            }
            str1++;
            str2++;
        }
        return (*str1 == '\0' && *str2 == '\0') ? 0 : ((*str1 == '\0') ? -1 : 1);
    }

    void* memset(void* ptr, int value, size_t num) {
        unsigned char* p = static_cast<unsigned char*>(ptr);
        for (size_t i = 0; i < num; i++) {
            p[i] = static_cast<unsigned char>(value);
        }
        return ptr;
    }

    char* strncpy(char* dest, const char* src, size_t n) {
        char* dest_ptr = dest;
        const char* src_ptr = src;

        // Copy at most n characters
        for (size_t i = 0; i < n; ++i) {
            if (*src_ptr != '\0') {
                *dest_ptr = *src_ptr;
                ++src_ptr;
            }
            else {
                // Pad destination with null characters
                *dest_ptr = '\0';
            }
            ++dest_ptr;
        }

        return dest;
    }
}

class Map {
private:
    struct Node {
        char key[MAX_CSTRING_SIZE];
        char value[MAX_CSTRING_SIZE];
        Node* next;
        size_t hashValue;
    };

    Node** buckets;
    size_t size;
    size_t capacity;
    
    // hash function for bucket determination
    // ensures constant lookup, insert, delete if no collisions 
    size_t hash(const char* key) {
        size_t hValue = 5381;
        size_t length = helpers::strlen(key);
        for (int i = 0; i < length; i++) {
            hValue = ((hValue << 5) + hValue) + key[i];
        }
        hValue += length;
        return hValue;
    }

    // copies the contents of map into a resized array, if the used space exceeds load factor
    void resize() {
        if (size >= LOAD_FACTOR * capacity) {
            size_t newCapacity = capacity * 2;
            Node** newBuckets = new Node * [newCapacity];

            helpers::memset(newBuckets, 0, newCapacity * sizeof(Node*));

            for (int i = 0; i < capacity; i++) {
                Node* node = buckets[i];
                if (node != nullptr) {
                    size_t index = node->hashValue % newCapacity;
                    newBuckets[index] = node;
                }
            }

            delete[] buckets;
            buckets = newBuckets;
            capacity = newCapacity;
        }
    }

    void setBuckets(Node** invBuckets, size_t invCapacity, size_t invSize) {
        clear();

        buckets = new Node * [invCapacity];
        helpers::memset(buckets, 0, invCapacity * sizeof(Node*));

        for (int i = 0; i < invCapacity; i++) {
            Node* invNode = invBuckets[i];
            Node* prevNode = nullptr;
            Node* newNode = nullptr;

            while (invNode != nullptr) {
                newNode = new Node();
                helpers::strncpy(newNode->key, invNode->key, MAX_CSTRING_SIZE);
                helpers::strncpy(newNode->value, invNode->value, MAX_CSTRING_SIZE);
                newNode->hashValue = invNode->hashValue;
                newNode->next = nullptr;

                if (prevNode == nullptr) {
                    buckets[i] = newNode;
                }
                else {
                    prevNode->next = newNode;
                }
                prevNode = newNode;

                invNode = invNode->next;
            }
        }

        capacity = invCapacity;
        size = invSize;
    }

public:
    Map() : size(0), capacity(DEFAULT_CAPACITY) {
        buckets = new Node * [capacity];
        helpers::memset(buckets, 0, capacity * sizeof(Node*));
    }

    ~Map() {
        clear();
    }

    void clear() {
        for (int i = 0; i < capacity; i++) {
            Node* node = buckets[i];
            if (node != nullptr) {
                delete node;
            }
        }
        delete[] buckets;
    }

    void put(const char* key, const char* value) {
        resize();

        size_t hashValue = hash(key);
        size_t index = hashValue % capacity;

        Node* node = buckets[index];

        if (node == nullptr) {
            Node* newNode = new Node();

            helpers::strncpy(newNode->key, key, MAX_CSTRING_SIZE);
            helpers::strncpy(newNode->value, value, MAX_CSTRING_SIZE);

            newNode->hashValue = hashValue;
            buckets[index] = newNode;
            size++;
            return;
        }

        // for bucket collision situations, get to the end of the linked list
        while (node->next != nullptr) {
            node = node->next;
        }

        Node* newNode = new Node();
        helpers::strncpy(newNode->key, key, MAX_CSTRING_SIZE);
        helpers::strncpy(newNode->value, value, MAX_CSTRING_SIZE);
        newNode->hashValue = hashValue;
        node->next = newNode;
        size++;

        return;
    }

    const char* get(const char* key) {
        size_t hashValue = hash(key);
        size_t index = hashValue % capacity;

        Node* node = buckets[index];

        while (node != nullptr) {
            if (helpers::strcmp(node->key, key) == 0) {
                return node->value;
            }
            node = node->next;
        }

        return nullptr;
    }

    void invert() {
        Map* inverted = new Map();

        for (int i = 0; i < capacity; i++) {
            Node* node = buckets[i];

            while (node != nullptr) {
                inverted->put(node->value, node->key);

                node = node->next;
            }
        }

        setBuckets(inverted->buckets, inverted->capacity, inverted->size);

        delete inverted;
    }
};

//#include <iostream>   
//#include <chrono>


int main() {
    //auto start = std::chrono::steady_clock::now();
    //C:/Users/hazya/OneDrive/Desktop/tulks.in
    freopen("C:/Users/hazya/OneDrive/Desktop/tulks.in", "r", stdin);
    freopen("tulks.out", "w", stdout);

    Map* map = new Map();
    char currentString[MAX_CSTRING_SIZE];

    scanf("%s", &currentString);

    // putting the contents of the file into Map
    while ((currentString[0] != '-') && (currentString[0] != '<')) {
        char val[MAX_CSTRING_SIZE];

        scanf("%s", &val);

        map->put(currentString, val);

        scanf("%s", &currentString);
    }

    // the direction ('<--' or '-->') signifies whether to following text from file will need to be found/replaced by
    // key or value (essentially should the keys and values should be logically swapped in the map)
    //bool direction = currentString[0] == '-' ? false : true;
    if (currentString[0] == '<') {
        map->invert();
    }

    // writing to output file
    bool lastString = false;
    while (scanf("%s", currentString) != EOF) {
        const char* value = map->get(currentString);

        if (value != nullptr) {
            printf("%s", value);
        }
        else {
            printf("?%s", currentString);
        }

        if (lastString) {
            printf("\n");
            break;
        }

        lastString = feof(stdin);
        if (!lastString) {
            printf(" ");
        }
    }

    //auto end = std::chrono::steady_clock::now();
    //std::chrono::duration<double> elapsed_seconds = end - start;
    //std::cout<< std::endl << "elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}
