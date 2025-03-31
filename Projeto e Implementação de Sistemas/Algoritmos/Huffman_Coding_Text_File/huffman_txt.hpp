#ifndef HUFFMAN_TXT_HPP 
#define HUFFMAN_TXT_HPP

#include "../Huffman/huffman.hpp"
#include <cstdint>
#include <cstdio>
#include <string>

class HuffmanTXT : public Huffman {
public:
    void count_duplicates(uint8_t* data); 
    char* encode(uint8_t* data);
    void compress();
    unsigned char* read_text();
    HuffmanTXT(std::string fp) : Huffman(fp) {};
    HuffmanTXT() : Huffman() {};
}; 

#endif //HUFFMAN_TXT_HPP
