#ifndef HUFFMAN_WAV_HPP 
#define HUFFMAN_WAV_HPP 

#include <cstdint>
#include <string>
#include "../formats/wav.hpp"
#include "huffman.hpp"

class HuffmanWAV : public Huffman {
private:
    wav wav_file;
public:
    void compress() override;  
    void decompress();
    HuffmanWAV(std::string fp) : Huffman(fp) {};
    HuffmanWAV() : Huffman() {};
}; 

#endif //HUFFMAN_WAV_HPP
