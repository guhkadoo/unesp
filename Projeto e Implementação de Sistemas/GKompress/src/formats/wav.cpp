#include "../include/formats/wav.hpp"
#include <cstdint>
#include <cstdio>

int wav::read(std::string filename)
{
    FILE *file = fopen(filename.c_str(), "rb");
    if(!file)
    {
        printf("Could not open file %s\n", filename.c_str());
        return -1;
    }
    if(fread(&riff_chunk_descriptor, sizeof(riff_chunk_descriptor), 1, file) != 1)
    {
        printf("Error reading RIFF header\n");
        fclose(file);
        return -1;
    }
    if(fread(&fmt_subchunk, sizeof(fmt_subchunk), 1, file) != 1)
    {
        printf("Error reading fmt subchunk\n");
        fclose(file);
        return -1;
    }
    if(fread(&data_subchunk, sizeof(data_subchunk), 1, file) != 1)
    {
        printf("Error reading data subchunk\n");
        fclose(file);
        return -1;
    }
    data = new uint8_t[data_subchunk.size];
    if(!data)
    {
        printf("Memory allocation failed for audio data\n");
        fclose(file);
        return -1;
    }
    if(fread(data, data_subchunk.size, 1, file) != 1)
    {
        printf("Error while reading audio data\n");
        free(data);
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

static void print_hex_array(char* arr)
{
    size_t i;
    for(i=0; i<4; i++)
        printf("%x ", arr[i]);
    printf("(");
    for(i=0; i<4; i++)
        printf("%c", arr[i]);
    printf(")\n");
}

void wav::print()
{
    printf("RIFF CHUNK DESCRIPTOR\n"
           "ID: ");
    print_hex_array(riff_chunk_descriptor.ID);
    printf("size: %x\n"
            "format: ",
            riff_chunk_descriptor.size);
    print_hex_array(riff_chunk_descriptor.format);


    printf("\"FMT\" SUBCHUNK\n"
           "ID: ");
    print_hex_array(fmt_subchunk.ID);
    printf("size: %x\n"
           "audio_format: %x\n"
           "num_channels: %x\n"
           "sample_rate: %x\n"
           "byte_rate: %x\n"
           "block_align: %x\n"
           "bits_per_sample: %x\n",
           fmt_subchunk.size,
           fmt_subchunk.audio_format,
           fmt_subchunk.num_channels,
           fmt_subchunk.sample_rate,
           fmt_subchunk.byte_rate,
           fmt_subchunk.block_align,
           fmt_subchunk.bits_per_sample);

    printf("\"DATA\" SUBCHUNK\n"
           "ID: ");
    print_hex_array(data_subchunk.ID);
    printf("size: %x\n", data_subchunk.size);
    for(size_t i=0; i<data_subchunk.size; i++)
        printf("%x ", data[i]);
    printf("\n");
}
