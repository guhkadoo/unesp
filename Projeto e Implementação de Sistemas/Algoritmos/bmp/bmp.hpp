#ifndef BMP_HPP
#define BMP_HPP

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstdint>

#define EXIT_WITH_ERROR(msg) (fprintf(stderr, "%s\n", msg), exit(1))

#pragma pack(push,1) 
class bmp {
public:
    struct HEADER {
        char signature[2];
        uint32_t file_size;
        uint32_t reserved; //unused
        uint32_t data_offset;
    } header;
    struct INFO_header {
        uint32_t size; //should be 40 or 0x28
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
    size_t data_size;

    int read(const char* filename);
    void print();
    uint32_t get_color_table_size();
    int has_color_table();
private:
    void read_bmp_data(FILE* file);
};
#pragma pack(pop)

#endif //BMP_HPP
