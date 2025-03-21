#include "wav.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int read_wav(const char* filename, wav* file_data)
{
    FILE *file = fopen(filename, "rb");
    if(!file)
    {
        printf("Could not open file %s\n", filename);
        return -1;
    }
    if(fread(&file_data->RIFF_chunk_descriptor, sizeof(file_data->RIFF_chunk_descriptor), 1, file) != 1)
    {
        printf("Error reading RIFF header\n");
        fclose(file);
        return -1;
    }
    if(fread(&file_data->fmt_subchunk, sizeof(file_data->fmt_subchunk), 1, file) != 1)
    {
        printf("Error reading fmt subchunk\n");
        fclose(file);
        return -1;
    }
    if(fread(&file_data->data_subchunk, sizeof(file_data->data_subchunk), 1, file) != 1)
    {
        printf("Error reading data subchunk\n");
        fclose(file);
        return -1;
    }
    file_data->data = (uint8_t*)malloc(file_data->data_subchunk.size);
    if(!file_data->data)
    {
        printf("Memory allocation failed for audio data\n");
        fclose(file);
        return -1;
    }
    if(fread(file_data->data, file_data->data_subchunk.size, 1, file) != 1)
    {
        printf("Error while reading audio data\n");
        free(file_data->data);
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

static void print_hex_array(char* arr)
{
    register int i;
    for(i=0; i<4; i++)
        printf("%x ", arr[i]);
    printf("(");
    for(i=0; i<4; i++)
        printf("%c", arr[i]);
    printf(")\n");
}

void print_wav(wav* file_data)
{
    printf("RIFF CHUNK DESCRIPTOR\n"
           "ID: ");
    print_hex_array(file_data->RIFF_chunk_descriptor.ID);
    printf("size: %x\n"
            "format: ", file_data->RIFF_chunk_descriptor.size);
    print_hex_array(file_data->RIFF_chunk_descriptor.format);


    printf("\"FMT\" SUBCHUNK\n"
           "ID: ");
    print_hex_array(file_data->fmt_subchunk.ID);
    printf("size: %x\n"
           "audio_format: %x\n"
           "num_channels: %x\n"
           "sample_rate: %x\n"
           "byte_rate: %x\n"
           "block_align: %x\n"
           "bits_per_sample: %x\n",
           file_data->fmt_subchunk.size,
           file_data->fmt_subchunk.audio_format,
           file_data->fmt_subchunk.num_channels,
           file_data->fmt_subchunk.sample_rate,
           file_data->fmt_subchunk.byte_rate,
           file_data->fmt_subchunk.block_align,
           file_data->fmt_subchunk.bits_per_sample);

    printf("\"DATA\" SUBCHUNK\n"
           "ID: ");
    print_hex_array(file_data->data_subchunk.ID);
    printf("size: %x\n", file_data->data_subchunk.size);
    for(register int i=0; i<file_data->data_subchunk.size; i++)
        printf("%x ", file_data->data[i]);
    printf("\n");
}
