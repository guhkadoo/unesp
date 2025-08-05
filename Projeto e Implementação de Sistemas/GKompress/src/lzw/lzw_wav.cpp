#include "../include/lzw/lzw_wav.hpp"
#include "../include/formats/wav.hpp"

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

void LZW_WAV::compress(int option) {
    wav_file.read(filepath);
    std::vector<std::pair<uint16_t, int>> codes = encode(wav_file.data, wav_file.data_subchunk.size);
    internal_compress(option, codes, write_wav_header, &wav_file);
}

int LZW_WAV::decompress(int option) {
    wav decompressed_wav;
    return internal_decompress(option, write_wav_header, read_wav_header, get_pos, &decompressed_wav);
}