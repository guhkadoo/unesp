#include "compression.h"
#include "../wav/wav.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

unsigned char *read_text();

int main(int argc, char *argv[])
{
    if(argc != 2)
        EXIT_WITH_ERROR("Digite apenas um argumento!");
// data structures:
    wav audio_file;
    List list;
    initialize_list(&list);
    char **dictionary;

    if(read_wav(argv[1], &audio_file) == -1)
        EXIT_WITH_ERROR("Error while reading .wav file");
    print_wav(&audio_file);

// ----- PART 1 - COUNT THE DUPLICATES -----
    count_duplicates(audio_file.data, audio_file.data_subchunk.size);
    //print_duplicates();

// ----- PART 2 - SORTED QUEUE -----
    fill_list(&list);
    //print_list(list);

// ----- PART 3 - HUFFMAN_TREE -----
    struct Node *huffman_tree = create_huffman_tree(&list);
    //print_huffman(huffman_tree, 1);

// ----- PART 4 - DICTIONARY -----
    int columns = height_huffman(huffman_tree);
    dictionary = dictionary_allocation(columns); 
    fill_dictionary(dictionary, huffman_tree, "", columns);
    //print_dictionary(dictionary);

// ----- PART 5 - ENCODING -----
    //printf("\033[31m");
    //for(register size_t i=0; i<audio_file.data_subchunk.size; i++)
    //    printf(B, BYTE_TO_BINARY(*(audio_file.data+i)));

    char *code = encode(dictionary, audio_file.data, audio_file.data_subchunk.size);
    //printf("\n\033[32m%s\n", code);

// ----- PART 6 - DECODING -----
    //char *decode_str = decode(code, huffman_tree);
    //    printf("\n\033[0m%s\n", decode_str);

// ----- PART 7 - COMPRESSING AND DECOMPRESSING -----
    compress(&audio_file, code);
    decompress(huffman_tree);
}
