#include <stdio.h>
#include "bmp.h"
#include <stdlib.h>
#include <stdint.h>

uint32_t get_color_table_size(bmp* bmp_file)
{
    info_header* ih = &(bmp_file->info_header);
    return 4 * (1 << ih->bits_per_pixel); 
}

uint32_t get_bmp_data_size(bmp* bmp_file)
{
    info_header* ih = &(bmp_file->info_header);
    header* h = &(bmp_file->header);
    return ih->width * ih->height * ih->bits_per_pixel / 8; 
}

int has_color_table(bmp* bmp_file)
{
    info_header* ih = &(bmp_file->info_header);
    if(ih->bits_per_pixel > 8)
        return 0;
    return 1;
}

int read_bmp(const char* filename, bmp* file_data)
{
    header* h = &(file_data->header);
    info_header* ih = &(file_data->info_header);
    FILE *file = fopen(filename, "rb");
    if(!file)
    {
        printf("Could not open file %s\n", filename);
        return -1;
    }
    if(fread(h, sizeof(*h), 1, file) != 1)
    {
        printf("Error reading bmp header\n");
        fclose(file);
        return -1;
    }
    if(fread(ih, sizeof(*ih), 1, file) != 1)
    {
        printf("Error reading bmp info_header\n");
        fclose(file);
        return -1;
    }
    if(!has_color_table(file_data))
    {
        file_data->color_table = NULL;
    } else {
        size_t color_table_size = (size_t)get_color_table_size(file_data); 
        file_data->color_table = (uint8_t*)malloc(color_table_size);

        if(fread(file_data->color_table, color_table_size, 1, file) != 1)
        {
            printf("Error while reading bmp color_table\n");
            fclose(file);
            return -1;
        }
    }

    size_t data_size = (size_t)get_bmp_data_size(file_data); 
    //printf("teste\nfile_size = %d\ninfo_header_size: %d\ndata_size: %d\n", h->file_size, ih->size, data_size); 
    file_data->data = (uint8_t*)malloc(data_size);

    if(fread(file_data->data, data_size, 1, file) != 1)
    {
        printf("Error while reading bmp data\n");
        fclose(file);
        return -1;
    }
    fclose(file);
}

void print_bmp(bmp* file_data)
{
    header* h = &(file_data->header);
    printf( "\nBMP HEADER:\n"
            "signature: %c%c\n"
            "file_size: %x\n"
            "reserved: %x\n"
            "data_offset: %x\n", 
            h->signature[0], h->signature[1],
            h->file_size, h->reserved, h->data_offset);

    info_header* ih = &(file_data->info_header);
    printf( "\nBMP INFO HEADER:\n"
            "size: %x\n"
            "width: %x\n"
            "height: %x\n"
            "planes: %x\n"
            "bits_per_pixel: %x\n"
            "compression: %x\n"
            "image_size: %x\n"
            "x_pixels_per_m: %x\n"
            "y_pixels_per_m: %x\n"
            "colors_used: %x\n"
            "important_colors: %x\n",
            ih->size, ih->width, ih-> height,
            ih->planes, ih->bits_per_pixel,
            ih->compression, ih->image_size,
            ih->x_pixels_per_m, ih->y_pixels_per_m,
            ih->colors_used, ih->important_colors);

    if(has_color_table(file_data))
    {
        size_t color_table_size = (size_t)get_color_table_size(file_data);
        printf( "\nBMP COLOR TABLE:\n");
        for(size_t i=0; i<color_table_size; i++)
            printf("%x ", file_data->color_table[i]);
        printf("\n");
    }

    size_t data_size = (size_t)get_bmp_data_size(file_data); 
    printf( "\nBMP DATA:\n");
    for(size_t i=0; i<data_size; i++)
        printf("%x ", file_data->data[i]);
    printf("\n");
    //https://entropymine.com/jason/bmpsuite/bmpsuite/html/bmpsuite.html
}
