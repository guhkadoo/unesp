#include "huffman_txt.hpp"
#include "../Huffman/huffman.hpp"
#include <string>

int main(int argc, char* argv[])
{
    std::string filepath = "archive.txt";
    
    HuffmanTXT archive(filepath);
    archive.compress();
    archive.decompress();

    return 0;
}
