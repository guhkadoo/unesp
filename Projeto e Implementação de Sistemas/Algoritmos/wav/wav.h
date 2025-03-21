#ifndef WAV_H
#define WAV_H

#include <stdint.h>

#pragma pack(push,1)
typedef struct {
    struct {
        char ID[4];
        uint32_t size;
        char format[4];
    } RIFF_chunk_descriptor;
    struct {
        char ID[4];
        uint32_t size;
        uint16_t audio_format;
        uint16_t num_channels;
        uint32_t sample_rate;
        uint32_t byte_rate;
        uint16_t block_align;
        uint16_t bits_per_sample;
    } fmt_subchunk;
    struct {
        char ID[4];
        uint32_t size;
    } data_subchunk;
    uint8_t* data; 
} wav;
#pragma pack(pop)

int read_wav(const char* filename, wav* file_data);
void print_wav(wav* file_data);

// https://www.mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/Samples.html

#endif //WAV_H
