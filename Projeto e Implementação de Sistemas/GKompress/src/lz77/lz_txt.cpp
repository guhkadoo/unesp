#include "../include/lz77/lz_txt.hpp"
#include <cstdint>
#include <cstddef>
#include <cstdio>

uint8_t* LZ77TXT::read_text(FILE* file) {
    uint8_t* data = nullptr;
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    data = new uint8_t[file_size];
    fread(data, 1, file_size, file);
    return data;
}

void LZ77TXT::compress(int option) {
    FILE* file = fopen(filepath.c_str(), "rb");
    if(!file) EXIT_WITH_ERROR("Error while executing read_text");
    uint8_t* data = read_text(file);
    fseek(file, 0, SEEK_END);
    size_t data_size = ftell(file);
    printf("data_size compress: %zu\n", data_size);
    internal_compress(data, data_size);
    delete[] data;

    fclose(file);
}