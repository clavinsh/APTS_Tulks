#include <stdio.h>

const int MAX_CSTRING_SIZE = 21;
const float LOAD_FACTOR = 0.75;

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

class Map {
private:
    static const int DEFAULT_CAPACITY = 16;

    struct Node {
        char key[MAX_CSTRING_SIZE];
        char value[MAX_CSTRING_SIZE];
        //int count;
        Node* next;
        size_t hashValue;
    };

    Node** buckets;
    int size;
    int capacity;

    // hash function for bucket determination
    // ensures constant lookup, insert, delete if no collisions 
    size_t hash(const char* key) {
        size_t hValue = 5381;
        int length = strlen(key);
        for (int i = 0; i < length; i++) {
            hValue = ((hValue << 5) + hValue) + key[i];
        }
        hValue += length;
        return hValue;
    }

    // copies the contents of map into a resized array, if the used space exceeds load factor
    void resize() {
        if (size >= LOAD_FACTOR * capacity) {
            int newCapacity = capacity * 2;
            Node** newBuckets = new Node * [newCapacity];

            memset(newBuckets, 0, newCapacity * sizeof(Node*));

            for (int i = 0; i < capacity; i++) {
                Node* node = buckets[i];
                if (node != nullptr) {
                    int index = node->hashValue % newCapacity;
                    newBuckets[index] = node;
                }
            }

            delete[] buckets;
            buckets = newBuckets;
            capacity = newCapacity;
        }
    }

    void setBuckets(Node** invBuckets, int invCapacity, int invSize) {
        clear();

        buckets = new Node * [invCapacity];
        memset(buckets, 0, invCapacity * sizeof(Node*));

        for (int i = 0; i < invCapacity; i++) {
            Node* invNode = invBuckets[i];
            Node* prevNode = nullptr;
            Node* newNode = nullptr;

            while (invNode != nullptr) {
                newNode = new Node();
                strncpy(newNode->key, invNode->key, MAX_CSTRING_SIZE);
                strncpy(newNode->value, invNode->value, MAX_CSTRING_SIZE);
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
        memset(buckets, 0, capacity * sizeof(Node*));
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
        int index = hashValue % capacity;

        Node* node = buckets[index];

        if (node == nullptr) {
            Node* newNode = new Node();

            strncpy(newNode->key, key, MAX_CSTRING_SIZE);
            strncpy(newNode->value, value, MAX_CSTRING_SIZE);

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
        strncpy(newNode->key, key, MAX_CSTRING_SIZE);
        strncpy(newNode->value, value, MAX_CSTRING_SIZE);
        newNode->hashValue = hashValue;
        node->next = newNode;
        size++;

        return;
    }

    const char* get(const char* key) {
        size_t hashValue = hash(key);
        int index = hashValue % capacity;

        Node* node = buckets[index];

        while (node != nullptr) {
            if (strcmp(node->key, key) == 0) {
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

int main() {
    freopen("tulks.in", "r", stdin);
    freopen("tulks.out", "w", stdout);

    Map* map = new Map();
    char currentString[MAX_CSTRING_SIZE];

    scanf("%s", &currentString);

    // putting the contents of the file into Map
    // || strcmp(currentString, "<--") != 0 || strcmp(currentString, "-->") != 0
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

    return 0;
}
