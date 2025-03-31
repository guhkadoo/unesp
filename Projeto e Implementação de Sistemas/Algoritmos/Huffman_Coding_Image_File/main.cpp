#include "huffman_bmp.hpp"
#include "../bmp/bmp.hpp"

int main(int argc, char *argv[])
{
    HuffmanBMP archive(argv[1]);
    archive.compress();
    archive.decompress();
}
