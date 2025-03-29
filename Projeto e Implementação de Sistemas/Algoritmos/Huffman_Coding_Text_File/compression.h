#ifndef COMPRESSION_H
#define COMPRESSION_H

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
void count_duplicates(unsigned char *str, unsigned int duplicates[]);
void print_duplicates(unsigned int duplicates[]);
// ----- PART 1 - END -----

// ----- PART 2 - SORTED QUEUE ----- 
struct Node {
    unsigned char character;
    int duplicates;
    struct Node *left, *right, *next;
};

typedef struct {
    struct Node *front;
    int size;
} List;

void initialize_list(List *list);
void insert_sorted(List *list, struct Node *node);
void fill_list(List *list, unsigned int duplicates[]);
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
char *encode(char **dictionary, unsigned char *text);
// ----- PART 5 - END -----

// ----- PART 6 - DECODING -----
char *decode(char *code, struct Node *tree);
// ----- PART 6 - END -----

// ----- PART 7 - COMPRESSING AND DECOMPRESSING -----
void compress(char *str);
void decompress(struct Node *tree);
// ----- PART 7 - END -----
unsigned char *read_text(char *file_name);

#endif //COMPRESSION_H
