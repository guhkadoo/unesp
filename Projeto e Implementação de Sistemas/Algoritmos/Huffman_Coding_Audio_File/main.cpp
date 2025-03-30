#include "huffman_wav.hpp"
#include "../Huffman/huffman.hpp"
#include "../wav/wav.hpp"

int main(int argc, char *argv[])
{
    HuffmanWAV archive(argv[1]);
    archive.compress();
    archive.decompress();

    return 0;
}
