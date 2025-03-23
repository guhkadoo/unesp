#include "compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//we're counting the duplicates of a string using an array which countains all the ascii (A to Z upper or lowercase).

// ----- PART 1 - COUNT THE DUPLICATES -----
void count_duplicates(unsigned char *str, unsigned int duplicates[])
{
    memset(duplicates, 0, SIZE*sizeof(int));
    for(int i=0; str[i]!='\0'; i++) {
        duplicates[str[i]]++;
    }
}

void print_duplicates(unsigned int duplicates[])
{
    for(int i=0; i<SIZE; i++) {
        if(duplicates[i] != 0) {
            printf("%c (%d) has %u duplicates\n", i, i, duplicates[i]);
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

void fill_list(List *list, unsigned int duplicates[])
{
    struct Node *new_node;
    for(int i=0; i<SIZE; i++) {
        if(duplicates[i] != 0) {
            new_node = (struct Node *)malloc(sizeof(struct Node));
            if(new_node != NULL) {
                new_node->character = (char)i;
                new_node->duplicates = duplicates[i];
                new_node->left = new_node->right = NULL;

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
        printf("character: %c    duplicates: %u\n", (list.front)->character, (list.front)->duplicates);
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
            new_node->character = '*';
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
    if(tree->left == NULL && tree->right == NULL) {
        printf("character: %c    duplicates: %u    height_tree: %d\n", tree->character, tree->duplicates, height);
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
        strcpy(dictionary[tree->character], code);
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
    for(int i=0; i<SIZE; i++) {
        if(strcmp(dictionary[i], "")) {
            printf("%c/%d: %s\n", i, i, dictionary[i]);
        }
    }
}
// ----- PART 4 - END -----

// ----- PART 5 - ENCODING -----
char *encode(char **dictionary, unsigned char *text)
{
    int size=0;
    for(int i=0; text[i]!='\0'; i++) {
        size += strlen(dictionary[text[i]]);
    }

    char *code = (char *)calloc(size+1, sizeof(char));
    for(int i=0; text[i]!='\0'; i++) {
        strcat(code, dictionary[text[i]]);
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
    for(int i=0; code[i] != '\0'; i++) {
        if(code[i] == '0') {
            traverse_node = traverse_node->left;
        } else if(code[i] == '1') {
            traverse_node = traverse_node->right;
        }

        if(traverse_node->left == NULL && traverse_node->right == NULL) {
            char concat[2]; concat[0] = traverse_node->character; concat[1] = '\0';
            strcat(decode, concat);

            traverse_node = tree;
        }
    }

    return decode;
}
// ----- PART 6 - END -----

// ----- PART 7 - COMPRESSING AND DECOMPRESSING -----
void compress(char *str)
{
    FILE *file = fopen("archive.GK", "wb");
    char byte = 0;
    if(file != NULL) {
        int j=7;
        for(int i=0; str[i] != '\0'; i++) {
            if(str[i] == '1')
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

void decompress(struct Node *tree)
{
    FILE *file = fopen("archive.GK", "rb");
    char byte;
    struct Node *aux = tree;

    if(file != NULL) {
        fseek(file, -1, SEEK_END);
        long last_byte_to_read_pos = ftell(file);
        fread(&byte, sizeof(char), 1, file);
        char quantity = byte;
        fseek(file, 0, SEEK_SET);
        long pos;
        int condition=0;

        while(fread(&byte, sizeof(char), 1, file)) {
            pos = ftell(file);
            if(pos == last_byte_to_read_pos + 1) {
                break;
            }
            for(int i=7; i>=condition; i--) {
                if(pos == last_byte_to_read_pos)
                    condition = 7-quantity+1;

                if(is_bit_1(byte, i)) {
                    aux = aux->right;
                } else {
                    aux = aux->left;
                }
                if(aux->right == NULL && aux->left == NULL) {
                    printf("%c", aux->character);
                    aux = tree;
                }
            } 
        } 
        fclose(file);
    } else {
        EXIT_WITH_ERROR("Error while opening/creating archive in void decompress(char *str)");
    }
}
// ----- PART 7 - END -----
