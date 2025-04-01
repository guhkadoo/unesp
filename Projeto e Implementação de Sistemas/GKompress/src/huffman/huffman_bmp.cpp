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

void HuffmanBMP::decompress() {
    size_t pos = sizeof(bmp_file.header) + sizeof(bmp_file.info_header);
    if(bmp_file.has_color_table())
        pos += bmp_file.get_color_table_size();
    Huffman::decompress(write_bmp_header, &bmp_file, pos);
}
