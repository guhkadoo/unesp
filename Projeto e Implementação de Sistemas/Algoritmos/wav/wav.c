#include "wav.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int read_wav(const char* filename, wav* file_data)
{
    RIFF_chunk_descriptor* rcd = &(file_data->RIFF_chunk_descriptor);
    fmt_subchunk* fmt = &(file_data->fmt_subchunk);
    data_subchunk* dsc = &(file_data->data_subchunk); 

    FILE *file = fopen(filename, "rb");
    if(!file)
    {
        printf("Could not open file %s\n", filename);
        return -1;
    }
    if(fread(rcd, sizeof(*rcd), 1, file) != 1)
    {
        printf("Error reading RIFF header\n");
        fclose(file);
        return -1;
    }
    if(fread(fmt, sizeof(*fmt), 1, file) != 1)
    {
        printf("Error reading fmt subchunk\n");
        fclose(file);
        return -1;
    }
    if(fread(dsc, sizeof(*dsc), 1, file) != 1)
    {
        printf("Error reading data subchunk\n");
        fclose(file);
        return -1;
    }
    file_data->data = (uint8_t*)malloc(dsc->size);
    if(!file_data->data)
    {
        printf("Memory allocation failed for audio data\n");
        fclose(file);
        return -1;
    }
    if(fread(file_data->data, dsc->size, 1, file) != 1)
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
    RIFF_chunk_descriptor* rcd = &(file_data->RIFF_chunk_descriptor);
    fmt_subchunk* fmt = &(file_data->fmt_subchunk);
    data_subchunk* dsc = &(file_data->data_subchunk); 

    printf("RIFF CHUNK DESCRIPTOR\n"
           "ID: ");
    print_hex_array(rcd->ID);
    printf("size: %x\n"
            "format: ",
            rcd->size);
    print_hex_array(rcd->format);


    printf("\"FMT\" SUBCHUNK\n"
           "ID: ");
    print_hex_array(fmt->ID);
    printf("size: %x\n"
           "audio_format: %x\n"
           "num_channels: %x\n"
           "sample_rate: %x\n"
           "byte_rate: %x\n"
           "block_align: %x\n"
           "bits_per_sample: %x\n",
           fmt->size,
           fmt->audio_format,
           fmt->num_channels,
           fmt->sample_rate,
           fmt->byte_rate,
           fmt->block_align,
           fmt->bits_per_sample);

    printf("\"DATA\" SUBCHUNK\n"
           "ID: ");
    print_hex_array(dsc->ID);
    printf("size: %x\n", dsc->size);
    for(register int i=0; i<dsc->size; i++)
        printf("%x ", file_data->data[i]);
    printf("\n");
}
