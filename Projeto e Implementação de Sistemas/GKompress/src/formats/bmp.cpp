#include <cstdio>
#include "../include/formats/bmp.hpp"
#include <cstdlib>
#include <cstdint>
#include <cstring>

uint32_t bmp::get_color_table_size()
{
    return 4 * (1 << info_header.bits_per_pixel); 
}

int bmp::has_color_table()
{
    if(info_header.bits_per_pixel > 8)
        return 0;
    return 1;
}

void bmp::read_bmp_data(FILE* file)
{
    size_t buffer_size = 1024;
    size_t var_data_size = 0;
    uint8_t* var_data = new uint8_t[buffer_size];
    if(!var_data) {
        fclose(file);
        EXIT_WITH_ERROR("Memory allocation failed");
    }
    size_t bytes_read;
    while((bytes_read = fread(var_data + var_data_size, 1, buffer_size-var_data_size, file)) > 0)
    {
        var_data_size += bytes_read;
        if(var_data_size >= buffer_size) // table doubling
        {
            buffer_size *= 2;
            uint8_t* new_data = (uint8_t*)realloc(var_data, buffer_size);
            if(!new_data)
            {
                free(var_data);
                fclose(file);
                EXIT_WITH_ERROR("Memory allocation failed");
            }
            var_data = new_data;
        }
    }
    data_size = var_data_size;

    data = (uint8_t*)malloc(data_size);
    if(!data)
    {
        free(data);
        fclose(file);
        EXIT_WITH_ERROR("Memory allocation failed");
    }
    memcpy(data, var_data, data_size);
    free(var_data);
}

int bmp::read(const char* filename)
{
    FILE *file = fopen(filename, "rb");
    if(!file)
    {
        printf("Could not open file %s\n", filename);
        return -1;
    }
    if(fread(&header, sizeof(header), 1, file) != 1)
    {
        printf("Error reading bmp header\n");
        fclose(file);
        return -1;
    }
    if(fread(&info_header, sizeof(info_header), 1, file) != 1)
    {
        printf("Error reading bmp info_header\n");
        fclose(file);
        return -1;
    }
    if(!has_color_table())
    {
        color_table = NULL;
    } else {
        size_t color_table_size = (size_t)get_color_table_size(); 
        color_table = new uint8_t[color_table_size];

        if(fread(color_table, color_table_size, 1, file) != 1)
        {
            printf("Error while reading bmp color_table\n");
            fclose(file);
            return -1;
        }
    }

    read_bmp_data(file);
    fclose(file);
    return 1;
}

void bmp::print()
{
    printf( "\nBMP HEADER:\n"
            "signature: %c%c\n"
            "file_size: %x\n"
            "reserved: %x\n"
            "data_offset: %x\n", 
            header.signature[0], header.signature[1],
            header.file_size, header.reserved, header.data_offset);

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
            info_header.size, info_header.width, info_header. height,
            info_header.planes, info_header.bits_per_pixel,
            info_header.compression, info_header.image_size,
            info_header.x_pixels_per_m, info_header.y_pixels_per_m,
            info_header.colors_used, info_header.important_colors);

    if(has_color_table())
    {
        size_t color_table_size = (size_t)get_color_table_size();
        printf( "\nBMP COLOR TABLE:\n");
        for(size_t i=0; i<color_table_size; i++)
            printf("%x ", color_table[i]);
        printf("\n");
    }

    printf( "\nBMP DATA:\n");
    printf("data_size: %ld                          // ADDED TO HEADER\n", data_size);
    for(size_t i=0; i<data_size; i++)
        printf("%x ", data[i]);
    printf("\n");
    //https://entropymine.com/jason/bmpsuite/bmpsuite/html/bmpsuite.html
}
