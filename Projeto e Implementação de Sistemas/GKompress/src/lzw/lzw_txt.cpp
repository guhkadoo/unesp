#include "../include/lzw/lzw_txt.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>

uint8_t* LZW_TXT::read_text() {
    FILE* file = fopen(filepath.c_str(), "rb");
    if (!file) EXIT_WITH_ERROR("Error opening text file");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);
    uint8_t* data = new uint8_t[file_size + 1];
    size_t read_count = fread(data, sizeof(uint8_t), file_size, file);
    data[read_count] = '\0';
    fclose(file);
    return data;
}

void LZW_TXT::compress(int option) {
    uint8_t* data = read_text();
    std::vector<std::pair<uint16_t, int>> codes = encode(data, strlen((char*)data));
    internal_compress(option, codes);
    delete[] data;
}

int LZW_TXT::decompress(int option) {
    return internal_decompress(option);
}