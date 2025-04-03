#ifndef HUFFMAN_BMP_HPP 
#define HUFFMAN_BMP_HPP 

#include <cstdint>
#include <cstdio>
#include <string>
#include "../formats/bmp.hpp"
#include "huffman.hpp"

class HuffmanBMP : public Huffman {
private:
    bmp bmp_file;
public:
    void compress(int option) override;
    int decompress(int option);
    HuffmanBMP(std::string fp) : Huffman(fp) {};
    HuffmanBMP(): Huffman() {};
};


#endif //HUFFMAN_BMP_HPP
