#include "compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
        EXIT_WITH_ERROR("Coloque apenas um argumento.\n");
// data structures:
    unsigned int duplicates[SIZE];
    List list;
    initialize_list(&list);
    char **dictionary;


    //int size = file_size();
    unsigned char *str = read_text(argv[1]);
    /* 
    unsigned char str[500];
    printf("Write a string to compress: ");
    scanf(" %499[^\n]", str);
    */

// ----- PART 1 - COUNT THE DUPLICATES -----
    count_duplicates(str, duplicates);
    //print_duplicates(duplicates);

// ----- PART 2 - SORTED QUEUE -----
    fill_list(&list, duplicates);
    //print_list(list);

// ----- PART 3 - HUFFMAN_TREE -----
    struct Node *huffman_tree = create_huffman_tree(&list);
    print_huffman(huffman_tree, 1);

// ----- PART 4 - DICTIONARY -----
    int columns = height_huffman(huffman_tree);
    dictionary = dictionary_allocation(columns); 
    fill_dictionary(dictionary, huffman_tree, "", columns);
//    print_dictionary(dictionary);

// ----- PART 5 - ENCODING -----
//    printf("\033[31m");
//    for(int i=0; str[i]!='\0'; i++)
//        printf(B, BYTE_TO_BINARY(str[i]));

    char *code = encode(dictionary, str);
//    printf("\n\033[32m%s\n", code);

// ----- PART 6 - DECODING -----
//   char *decode_str = decode(code, huffman_tree);
//    printf("\n\033[0m%s\n", decode_str);

// ----- PART 7 - COMPRESSING AND DECOMPRESSING -----
    compress(code);
    decompress(huffman_tree);
}
