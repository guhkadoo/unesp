#include <stdio.h>
#include "bmp.h"
#include <stdint.h>

int main(int argc, char* argv[])
{
    bmp bmp_file;
    if(argc != 2)
        EXIT_WITH_ERROR("Only 1 argument pls");
    if(read_bmp(argv[1], &bmp_file) != -1)
        printf("bmp file read!\n");
    print_bmp(&bmp_file);

    return 0;
}
