#include <stdio.h>
#include "bmp.h"
#include <stdlib.h>
#include <stdint.h>

int read_bmp(const char* filename, bmp* file_data)
{
    FILE *file = fopen(filename, "rb");
    if(!file)
    {
        printf("Could not open file %s\n", filename);
        return -1;
    }
    if(fread(&file_data->header, sizeof(file_data->header), 1, file) != 1)
    {
        printf("Error reading bmp header\n");
        fclose(file);
        return -1;
    }
    if(fread(&file_data->info_header, sizeof(file_data->info_header), 1, file) != 1)
    {
        printf("Error reading bmp info_header\n");
        fclose(file);
        return -1;
    }

    //size_t data_size = file_data->info_header.width * file_data->info_header.height * file_data->info_header.bits_per_pixel/8;

    size_t sum = sizeof(file_data->header) + sizeof(file_data->info_header);
    size_t data_size = file_data->header.file_size - sum; 
    printf("data_offset: %d\nsizeof(bmp.header) + sizeof(info_header): %d\ndata_size: %d\n",
            file_data->header.data_offset, sum, data_size);

    printf("teste\nfile_size = %d\ninfo_header_size: %d\n", file_data->header.file_size, file_data->info_header.size); 

    if(sum == file_data->header.data_offset)
    {
        file_data->color_table = NULL;
    } else {
        size_t color_table_size = 4 * file_data->info_header.bits_per_pixel;
        printf("color_table_size: %d\n", color_table_size);
        if(fread(file_data->color_table, color_table_size, 1, file) != 1)
        {
            printf("Error while reading bmp color_table\n");
            fclose(file);
            return -1;
        }
    }

    if(fread(file_data->data, data_size, 1, file) != 1)
    {
        printf("Error while reading bmp data\n");
        fclose(file);
        return -1;
    }
    fclose(file);
}

