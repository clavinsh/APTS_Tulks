//#include <stdio.h>
#include <iostream>

struct Map {
    struct Node {
        char key[21];
        char value[21];
        Node* next;
    };




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

    while (!std::feof(input_file)) {
        scanf_s("%s", currentString, (unsigned)_countof(currentString));
        printf(currentString);
    }


    // tālāk noskaidro virzienu --> vai <--

    // tālāk iet pa vardam no fin caur map un beigās izved arā fout (katru vārdu atsevisķi)

    return 0;
}

