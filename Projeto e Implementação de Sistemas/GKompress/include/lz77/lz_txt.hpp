#ifndef LZ77_TXT_HPP
#define LZ77_TXT_HPP

#include "../lz77/lz.hpp"
#include <cstdint>
#include <string>

class LZ77TXT : public LZ77 {
public:
    LZ77TXT() : LZ77() {}
    LZ77TXT(std::string fp) : LZ77(fp) {}
    void compress(int option) override;
    uint8_t* read_text(FILE* file);
};

#endif // LZ77_TXT_HPP
