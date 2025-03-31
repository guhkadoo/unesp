#ifndef HUFFMAN_BMP_HPP 
#define HUFFMAN_BMP_HPP 

#include <cstdint>
#include <cstdio>
#include <string>
#include "../bmp/bmp.hpp"
#include "../Huffman/huffman.hpp"

class HuffmanBMP : public Huffman {
private:
    bmp bmp_file;
public:
    void compress() override;
    void decompress();
    HuffmanBMP(std::string fp) : Huffman(fp) {};
};


#endif //HUFFMAN_BMP_HPP
