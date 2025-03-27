#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stdint.h>
#include "../wav/wav.h"

#define SIZE 256
#define EXIT_WITH_ERROR(msg) (fprintf(stderr, "%s\n", msg), exit(1))

#define B "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte) \
    ((byte) & 0x80 ? '1' : '0'), \
    ((byte) & 0x40 ? '1' : '0'), \
    ((byte) & 0x20 ? '1' : '0'), \
    ((byte) & 0x10 ? '1' : '0'), \
    ((byte) & 0x08 ? '1' : '0'), \
    ((byte) & 0x04 ? '1' : '0'), \
    ((byte) & 0x02 ? '1' : '0'), \
    ((byte) & 0x01 ? '1' : '0')

// ----- PART 1 - COUNT THE DUPLICATES -----
void count_duplicates(uint8_t *data, uint32_t data_size);
void print_duplicates();
// ----- PART 1 - END -----

// ----- PART 2 - SORTED QUEUE ----- 
struct Node {
    uint8_t byte;
    uint32_t duplicates;
    struct Node *left, *right, *next;
};

typedef struct {
    struct Node *front;
    uint32_t size;
} List;

void initialize_list(List *list);
void insert_sorted(List *list, struct Node *node);
void fill_list(List *list);
void print_list(List list);
// ----- PART 2 - END -----

// ----- PART 3 - HUFFMAN TREE -----
struct Node *remove_front(List *list);
struct Node *create_huffman_tree(List *list);
void print_huffman(struct Node *tree, int height);
// ----- PART 3 - END -----

// ----- PART 4 - DICTIONARY -----
int height_huffman(struct Node *tree);
char **dictionary_allocation(int j);
void fill_dictionary(char **dictionary, struct Node *tree, char *code, int columns);
void print_dictionary(char **dictionary);
// ----- PART 4 - END -----

// ----- PART 5 - ENCODING -----
char *encode(char **dictionary, uint8_t *data, uint32_t data_size);
// ----- PART 5 - END -----

// ----- PART 6 - DECODING -----
char *decode(char *code, struct Node *tree);
// ----- PART 6 - END -----

// ----- PART 7 - COMPRESSING AND DECOMPRESSING -----
void compress(wav *audio_file, char *code);
void decompress(struct Node *tree);
// ----- PART 7 - END -----

#endif //COMPRESSION_H
