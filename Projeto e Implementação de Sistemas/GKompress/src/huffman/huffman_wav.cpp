#include "../include/huffman/huffman_wav.hpp"
#include "../include/huffman/huffman.hpp"
#include "../include/formats/wav.hpp"

static void write_wav_header(FILE* file, void* audio_file)
{
    wav* wav_ptr = static_cast<wav*>(audio_file);
    fwrite(&wav_ptr->riff_chunk_descriptor, sizeof(wav_ptr->riff_chunk_descriptor), 1, file);
    fwrite(&wav_ptr->fmt_subchunk, sizeof(wav_ptr->fmt_subchunk), 1, file);
    fwrite(&wav_ptr->data_subchunk, sizeof(wav_ptr->data_subchunk), 1, file);
}

static void read_wav_header(FILE* file, void* audio_file)
{
    wav* wav_ptr= static_cast<wav*>(audio_file); 
    fread(&wav_ptr->riff_chunk_descriptor, sizeof(wav_ptr->riff_chunk_descriptor), 1, file);
    fread(&wav_ptr->fmt_subchunk, sizeof(wav_ptr->fmt_subchunk), 1, file);
    fread(&wav_ptr->data_subchunk, sizeof(wav_ptr->data_subchunk), 1, file);
}

static size_t get_pos(void* wav_file)
{
    wav* wav_ptr= static_cast<wav*>(wav_file); 
    size_t pos = sizeof(wav_ptr->riff_chunk_descriptor) + sizeof(wav_ptr->fmt_subchunk) + sizeof(wav_ptr->data_subchunk); 
    return pos;
}

void HuffmanWAV::compress() 
{
    wav_file.read(filepath);
    count_duplicates(wav_file.data, wav_file.data_subchunk.size);
    list.fill(duplicates);
    tree.build(list);
    size_t max_columns_of_dictionary = tree.height();
    dictionary.build(max_columns_of_dictionary);
    dictionary.fill(tree.root, "", max_columns_of_dictionary);
    char* code = encode(wav_file.data, wav_file.data_subchunk.size);
    internal_compress(code, write_wav_header, &wav_file);
}

void HuffmanWAV::decompress()
{
    wav decompressed_wav;
    Huffman::decompress(write_wav_header, read_wav_header, get_pos, &decompressed_wav);
}
