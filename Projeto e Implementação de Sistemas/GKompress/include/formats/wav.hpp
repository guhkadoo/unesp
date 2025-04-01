#ifndef WAV_HPP
#define WAV_HPP

#include <cstdint>
#include <string>

#pragma pack(push,1)
class wav {
public:
    struct RIFF_chunk_descriptor {
        char ID[4];
        uint32_t size;
        char format[4];
    } riff_chunk_descriptor;
    struct FMT_subchunk {
        char ID[4];
        uint32_t size;
        uint16_t audio_format;
        uint16_t num_channels;
        uint32_t sample_rate;
        uint32_t byte_rate;
        uint16_t block_align;
        uint16_t bits_per_sample;
    } fmt_subchunk;
    struct DATA_subchunk {
        char ID[4];
        uint32_t size;
    } data_subchunk;
    uint8_t* data; 

    int read(std::string filename);
    void print();
};
#pragma pack(pop)


// https://www.mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/Samples.html

#endif //WAV_HPP
