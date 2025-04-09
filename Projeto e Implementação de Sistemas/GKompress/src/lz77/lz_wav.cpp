#include "../include/lz77/lz_wav.hpp"
#include "../include/formats/wav.hpp"
#include <cstdint>
#include <cstddef>
#include <cstdio>

static void write_wav_header(FILE* file, void* audio_file) {
    wav* wav_ptr = static_cast<wav*>(audio_file);
    fwrite(&wav_ptr->riff_chunk_descriptor, sizeof(wav_ptr->riff_chunk_descriptor), 1, file);
    fwrite(&wav_ptr->fmt_subchunk, sizeof(wav_ptr->fmt_subchunk), 1, file);
    fwrite(&wav_ptr->data_subchunk, sizeof(wav_ptr->data_subchunk), 1, file);
}

static void read_wav_header(FILE* file, void* audio_file) {
    wav* wav_ptr = static_cast<wav*>(audio_file);
    fread(&wav_ptr->riff_chunk_descriptor, sizeof(wav_ptr->riff_chunk_descriptor), 1, file);
    fread(&wav_ptr->fmt_subchunk, sizeof(wav_ptr->fmt_subchunk), 1, file);
    fread(&wav_ptr->data_subchunk, sizeof(wav_ptr->data_subchunk), 1, file);
}

static size_t get_pos(void* wav_file) {
    wav* wav_ptr = static_cast<wav*>(wav_file);
    return sizeof(wav_ptr->riff_chunk_descriptor) + sizeof(wav_ptr->fmt_subchunk) + sizeof(wav_ptr->data_subchunk);
}

void LZ77WAV::compress(int option) {
    wav_file.read(filepath);
    internal_compress(wav_file.data, wav_file.data_subchunk.size, write_wav_header, &wav_file);
}
