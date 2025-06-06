#include "../include/lz77/lz_bmp.hpp"
#include "../include/formats/bmp.hpp"
#include <cstdint>
#include <cstddef>
#include <cstdio>

static void write_bmp_header(FILE* file, void* image_file) {
    bmp* bmp_ptr = static_cast<bmp*>(image_file);
    fwrite(&bmp_ptr->header, sizeof(bmp_ptr->header), 1, file);
    fwrite(&bmp_ptr->info_header, sizeof(bmp_ptr->info_header), 1, file);
    if (bmp_ptr->has_color_table()) {
        fwrite(bmp_ptr->color_table, bmp_ptr->get_color_table_size(), 1, file);
    }
}

static void read_bmp_header(FILE* file, void* image_file) {
    bmp* bmp_ptr = static_cast<bmp*>(image_file);
    fread(&bmp_ptr->header, sizeof(bmp_ptr->header), 1, file);
    fread(&bmp_ptr->info_header, sizeof(bmp_ptr->info_header), 1, file);
    printf("estamos em %d\n", ftell(file));
    if (bmp_ptr->has_color_table()) {
        size_t color_table_size = bmp_ptr->get_color_table_size();
        bmp_ptr->color_table = new uint8_t[color_table_size];    
        fread(bmp_ptr->color_table, bmp_ptr->get_color_table_size(), 1, file);
    }
}

static size_t get_pos(void* image_file) {
    bmp* bmp_ptr = static_cast<bmp*>(image_file);
    size_t seek_pos = sizeof(bmp_ptr->header) + sizeof(bmp_ptr->info_header);
    if (bmp_ptr->has_color_table()) {
        seek_pos += bmp_ptr->get_color_table_size();
    }
    return seek_pos;
}

void LZ77BMP::compress(int option) {
    bmp_file.read(filepath.c_str());
    internal_compress(bmp_file.data, bmp_file.data_size, write_bmp_header, &bmp_file);
}

int LZ77BMP::decompress(int option)
{
    bmp decompressed_bmp;
    return LZ77::decompress(option, write_bmp_header, read_bmp_header, get_pos, &decompressed_bmp);
}
