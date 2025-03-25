#include "compression.h"
#include "../wav/wav.h"
#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>
#include <string.h>

//we're counting the duplicates of a string using an array which countains all the ascii (A to Z upper or lowercase).

// ----- PART 1 - COUNT THE DUPLICATES -----
static uint32_t duplicates[SIZE]; 

void count_duplicates(uint8_t *data, uint32_t data_size)
{
    memset(duplicates, 0, SIZE*sizeof(uint32_t));
    for(register size_t i=0; i<data_size; i++) {
        duplicates[data[i]]++;
    }
}

void print_duplicates()
{
    for(register size_t i=0; i<SIZE; i++) {
        if(duplicates[i] != 0) {
            printf("%c (%d) has %llu duplicates\n", i, i, duplicates[i]);
        }
    }
}
// ----- PART 1 - END -----

// ----- PART 2 - SORTED QUEUE ----- 
void initialize_list(List *list)
{
    list->front = NULL;
    list->size = 0;
}

void insert_sorted(List *list, struct Node *node)
{
    if(list->front == NULL) {
        list->front = node;
    } else if(node->duplicates < list->front->duplicates) {
        node->next = list->front;
        list->front = node;
    } else {
        struct Node *aux = list->front;
        while(aux->next && aux->next->duplicates < node->duplicates) {
            aux = aux->next;
        }
        node->next = aux->next;
        aux->next = node;
    }
    list->size++;
}

void fill_list(List *list)
{
    struct Node *new_node;
    for(register size_t i=0; i<SIZE; i++) {
        if(duplicates[i] != 0) {
            new_node = (struct Node *)malloc(sizeof(struct Node));
            if(new_node != NULL) {
                new_node->byte = (uint8_t)i;
                new_node->duplicates = duplicates[i];
                new_node->next = new_node->left = new_node->right = NULL;

                insert_sorted(list, new_node);
            } else {
                EXIT_WITH_ERROR("Error while allocating memory.");
            }
        }
    }
}

void print_list(List list)
{
    printf("Sorted list of size %d:\n", list.size);
    while(list.front) {
        printf("byte: %u    duplicates: %u\n", (list.front)->byte, (list.front)->duplicates);
        list.front = (list.front)->next;
    }
    printf("\n");
}
// ----- PART 2 - END -----

// ----- PART 3 - HUFFMAN TREE -----
struct Node *remove_front(List *list)
{
    struct Node *remove = NULL;
    if(list->front != NULL) {
        remove = list->front;
        list->front = remove->next;
        remove->next = NULL;
        list->size--;
    }
    return remove;
}

struct Node *create_huffman_tree(List *list)
{
    struct Node *first_removed, *second_removed, *new_node;
    while(list->size > 1) {
        first_removed = remove_front(list);
        second_removed = remove_front(list);
        new_node = (struct Node *)malloc(sizeof(struct Node));

        if(new_node != NULL) {
            new_node->byte = '*';
            new_node->duplicates = first_removed->duplicates + second_removed->duplicates;
            new_node->left = first_removed;
            new_node->right = second_removed;
            new_node->next = NULL;

            insert_sorted(list, new_node);
        } else {
            EXIT_WITH_ERROR("Error while allocating memory.");
        }
    }
    return list->front;
}

void print_huffman(struct Node *tree, int height)
{
    if(tree == NULL) return;
    if(tree->left == NULL && tree->right == NULL) {
        printf("character: %c    duplicates: %u    height_tree: %d\n", tree->byte, tree->duplicates, height);
    } else {
        print_huffman(tree->left, height + 1);
        print_huffman(tree->right, height + 1);
    }
}
// ----- PART 3 - END -----

// ----- PART 4 - DICTIONARY -----
int height_huffman(struct Node *tree)
{
    if(tree == NULL)
        return -1;
    int left = height_huffman(tree->left);
    int right = height_huffman(tree->right);
    return (right > left) ? (right + 1) : (left + 1);
}

char **dictionary_allocation(int j)
{
    char **dictionary = (char **)malloc(SIZE * sizeof(char *));
    for(int i=0; i<SIZE; i++)
        dictionary[i] = (char *)calloc(j+1, sizeof(char));
    
    return dictionary;
}

void fill_dictionary(char **dictionary, struct Node *tree, char *code, int columns)
{
    char left[columns+1], right[columns+1];
    if(tree->left == NULL && tree->right == NULL) {
        strcpy(dictionary[tree->byte], code);
    } else {
        strcpy(left, code);
        strcpy(right, code);

        strcat(left, "0");
        strcat(right, "1");

        fill_dictionary(dictionary, tree->left, left, columns);
        fill_dictionary(dictionary, tree->right, right, columns);
    }
}

void print_dictionary(char **dictionary)
{
    for(register size_t i=0; i<SIZE; i++) {
        if(strcmp(dictionary[i], "")) {
            printf("%c/%d: %s\n", i, i, dictionary[i]);
        }
    }
}
// ----- PART 4 - END -----

// ----- PART 5 - ENCODING -----

static inline char *strcat_endpointer(char *dest, char *src)
{
    while(*dest) dest++;
    while(*dest++ = *src++);
    return --dest;
}

char *encode(char **dictionary, uint8_t *data, uint32_t data_size)
{
    int size=0;
    register size_t i;
    for(i=0; i<data_size; i++) {
        size += strlen(dictionary[data[i]]);
    }

    char *code = (char *)malloc(size+1);
    if(!code) EXIT_WITH_ERROR("Error while allocating memory for char *code");
    char *ptr = code;
    for(i=0; i<data_size; i++) {
        ptr = strcat_endpointer(ptr, dictionary[data[i]]); 
    }

    return code;
}
// ----- PART 5 - END -----

// ----- PART 6 - DECODING -----
char *decode(char *code, struct Node *tree)
{
    const int size = strlen(code);
    char *decode = (char *)calloc(size+1, sizeof(char));

    struct Node *traverse_node = tree;
    for(register size_t i=0; code[i] != '\0'; i++) {
        if(code[i] == '0') {
            traverse_node = traverse_node->left;
        } else if(code[i] == '1') {
            traverse_node = traverse_node->right;
        }

        if(traverse_node->left == NULL && traverse_node->right == NULL) {
            uint8_t concat[2]; concat[0] = traverse_node->byte; concat[1] = '\0';
            strcat(decode, concat);

            traverse_node = tree;
        }
    }

    return decode;
}
// ----- PART 6 - END -----

// ----- PART 7 - COMPRESSING AND DECOMPRESSING -----
void compress(wav *audio_file, char *code)
{
    FILE *file = fopen("archive.GK", "wb");
    uint8_t byte = 0;
    if(file != NULL) {
        // Add WAV header
        // RIFF Chunk Descriptor
        fwrite(&audio_file->RIFF_chunk_descriptor, sizeof(audio_file->RIFF_chunk_descriptor), 1, file);
        // fmt Subchunk
        fwrite(&audio_file->fmt_subchunk, sizeof(audio_file->fmt_subchunk), 1, file);
        // data Subchunk
        fwrite(&audio_file->data_subchunk, sizeof(audio_file->data_subchunk), 1, file);

        int j=7;
        for(int i=0; code[i] != '\0'; i++) {
            if(code[i] == '1')
                byte |= (1U << j); 
            j--;
            if(j < 0) {
                fwrite(&byte, sizeof(char), 1, file);
                byte = 0;
                j = 7;
            }
        }
        if(j != 7) {
            fwrite(&byte, sizeof(char), 1, file);
            byte = 7-j; // we will add a last byte to the final because we need to know where to stop if the byte is not multiple of 8
                        // as a consequence, we write the number of elements to be read (byte = 7-j)
            fwrite(&byte, sizeof(char), 1, file);
        } else {
            byte = 7; 
            fwrite(&byte, sizeof(char), 1, file);
        }
        fclose(file);
    } else {
        EXIT_WITH_ERROR("Error while opening/creating archive in void compress(char *str)");
    }
}

static int is_bit_1(char byte, int i)
{
    return byte &= (1U << i);
}
void decompress(struct Node *tree) {
    FILE *file = fopen("archive.GK", "rb");
    FILE *out = fopen("decompressed.wav", "wb");

    if (file == NULL || out == NULL) {
        EXIT_WITH_ERROR("Error opening files in decompress");
    }

    wav audio_file;

    // Read and write the WAV header
    fread(&audio_file.RIFF_chunk_descriptor, sizeof(audio_file.RIFF_chunk_descriptor), 1, file);
    fread(&audio_file.fmt_subchunk, sizeof(audio_file.fmt_subchunk), 1, file);
    fread(&audio_file.data_subchunk, sizeof(audio_file.data_subchunk), 1, file);
    fwrite(&audio_file.RIFF_chunk_descriptor, sizeof(audio_file.RIFF_chunk_descriptor), 1, out);
    fwrite(&audio_file.fmt_subchunk, sizeof(audio_file.fmt_subchunk), 1, out);
    fwrite(&audio_file.data_subchunk, sizeof(audio_file.data_subchunk), 1, out);

    // Get the last byte position
    fseek(file, -1, SEEK_END);
    long last_byte_to_read_pos = ftell(file);
    char quantity;
    fread(&quantity, sizeof(char), 1, file);
    fseek(file, sizeof(audio_file.RIFF_chunk_descriptor) + 
                sizeof(audio_file.fmt_subchunk) + 
                sizeof(audio_file.data_subchunk), SEEK_SET);

    // Decompression process
    struct Node *aux = tree;
    char byte;
    long pos;
    int condition = 0;

    while (fread(&byte, sizeof(char), 1, file)) {
        pos = ftell(file);
        if (pos == last_byte_to_read_pos + 1) {
            break;
        }
        for (int i = 7; i >= condition; i--) {
            if (pos == last_byte_to_read_pos) {
                condition = 7 - quantity + 1;
            }

            // Traverse Huffman Tree
            aux = is_bit_1(byte, i) ? aux->right : aux->left;

            // When reaching a leaf node, write decoded byte
            if (aux->right == NULL && aux->left == NULL) {
                fwrite(&aux->byte, sizeof(char), 1, out);
                aux = tree;
            }
        }
    }

    fclose(file);
    fclose(out);
}
// ----- PART 7 - END -----
