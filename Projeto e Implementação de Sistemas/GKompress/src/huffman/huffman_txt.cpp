#include "../include/huffman/huffman_txt.hpp"
#include "../include/huffman/huffman.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

void HuffmanTXT::count_duplicates(uint8_t* data) 
{
    for(int i=0; data[i]!='\0'; i++)
        duplicates[data[i]]++;
}

char* HuffmanTXT::encode(uint8_t* data)
{
    size_t size=0, i;
    for(i=0; data[i]!='\0'; i++) {
        size += strlen(dictionary[data[i]]);
    }

    char *code = (char *)calloc(size+1, sizeof(char));
    for(i=0; data[i]!='\0'; i++) {
        strcat(code, dictionary[data[i]]);
    }

    return code;
}

void HuffmanTXT::compress()
{
    uint8_t* data = read_text();
    count_duplicates(data);
    list.fill(duplicates);
    tree.build(list);
    size_t max_columns_of_dictionary = tree.height();
    dictionary.build(max_columns_of_dictionary);
    dictionary.fill(tree.root, "", max_columns_of_dictionary);
    char* code = encode(data);
    internal_compress(code);
}

unsigned char* HuffmanTXT::read_text()
{
    FILE *file = fopen(filepath.c_str(), "rb");
    unsigned char *str;
    if(file != NULL) {
        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        rewind(file);

        str = new unsigned char[file_size]();
        size_t read_count = fread(str, sizeof(char), file_size, file);
        str[read_count] = '\0';

        fclose(file);
    } else {
        EXIT_WITH_ERROR("Error while executing read_text");
    }

    return str;
}
