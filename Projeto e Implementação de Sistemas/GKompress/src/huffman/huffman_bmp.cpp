#include "../include/huffman/huffman_bmp.hpp"
#include "../include/huffman/huffman.hpp"
#include "../include/formats/bmp.hpp"
#include <cstdio>
#include <cstdint> 
#include <cstdlib>
#include <cstring>

static void write_bmp_header(FILE* file, void* image_file)
{
    bmp* bmp_ptr = static_cast<bmp*>(image_file); 
    fwrite(&bmp_ptr->header, sizeof(bmp_ptr->header), 1, file);
    fwrite(&bmp_ptr->info_header, sizeof(bmp_ptr->info_header), 1, file);
    if(bmp_ptr->has_color_table())
    {
        fwrite(bmp_ptr->color_table, bmp_ptr->get_color_table_size(), 1, file);
    }
}

static void read_bmp_header(FILE* file, void* image_file)
{
    bmp* bmp_ptr = static_cast<bmp*>(image_file); 
    fread(&bmp_ptr->header, sizeof(bmp_ptr->header), 1, file);
    fread(&bmp_ptr->info_header, sizeof(bmp_ptr->info_header), 1, file);
    if(bmp_ptr->has_color_table())
    {
        fwrite(bmp_ptr->color_table, bmp_ptr->get_color_table_size(), 1, file);
    }
}

static size_t get_pos(void* image_file)
{
    bmp* bmp_ptr = static_cast<bmp*>(image_file);
    size_t seek_pos = sizeof(bmp_ptr->header) + sizeof(bmp_ptr->info_header);
        if(bmp_ptr->has_color_table())
            seek_pos += bmp_ptr->get_color_table_size();
    return seek_pos;
}

void HuffmanBMP::compress()
{
    bmp_file.read(filepath.c_str());
    count_duplicates(bmp_file.data, bmp_file.data_size);
    list.fill(duplicates);
    tree.build(list);
    size_t max_columns_of_dictionary = tree.height();
    dictionary.build(max_columns_of_dictionary);
    dictionary.fill(tree.root, "", max_columns_of_dictionary);
    char* code = encode(bmp_file.data, bmp_file.data_size);
    internal_compress(code, write_bmp_header, &bmp_file);
}

int HuffmanBMP::decompress() {
    bmp decompressed_bmp;
    return Huffman::decompress(write_bmp_header, read_bmp_header, get_pos, &decompressed_bmp);
}
