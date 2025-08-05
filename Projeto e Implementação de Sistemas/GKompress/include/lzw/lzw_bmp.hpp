#ifndef LZW_BMP_HPP
#define LZW_BMP_HPP

#include "lzw/lzw.hpp"
#include "formats/bmp.hpp"

class LZW_BMP : public LZW {
private:
    bmp bmp_file;

public:
    LZW_BMP() = default;
    LZW_BMP(std::string fp) : LZW() { filepath = fp; }
    void compress(int option) override;
    int decompress(int option) override;
};

#endif