#include <cstdio>
#include "bmp.hpp"
#include <cstdint>

int main(int argc, char* argv[])
{
    bmp bmp_file;
    if(argc != 2)
        EXIT_WITH_ERROR("Only 1 argument pls");
    if(bmp_file.read(argv[1]) != -1)
        printf("bmp file read!\n");
    bmp_file.print();

    return 0;
}
