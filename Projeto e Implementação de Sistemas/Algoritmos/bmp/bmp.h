#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stdlib.h>

#define EXIT_WITH_ERROR(msg) (fprintf(stderr, "%s\n", msg), exit(1))

#pragma pack(push,1) 
typedef struct {
    struct {
        char signature[2];
        uint32_t file_size;
        uint32_t reserved; //unused
        uint32_t data_offset;
    } header;
    struct {
        uint32_t size; //should be 40
        uint32_t width;
        uint32_t height;
        uint16_t planes;
        uint16_t bits_per_pixel;
        uint32_t compression;
        uint32_t image_size;
        uint32_t x_pixels_per_m;
        uint32_t y_pixels_per_m;
        uint32_t colors_used;
        uint32_t important_colors;
    } info_header;
    uint8_t *color_table;
    uint8_t *data;
} bmp;
#pragma pack(pop)

int read_bmp(const char* filename, bmp* file_data);
void print_bmp(bmp* file_data);
#endif //BMP_H
