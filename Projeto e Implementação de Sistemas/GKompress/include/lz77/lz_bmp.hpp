#ifndef LZ77_BMP_HPP
#define LZ77_BMP_HPP

#include "../lz77/lz.hpp"
#include "../formats/bmp.hpp"
#include <string>

class LZ77BMP : public LZ77 {
private:
    bmp bmp_file;
public:
    LZ77BMP() : LZ77() {}
    LZ77BMP(std::string fp) : LZ77(fp) {}
    void compress(int option) override;
};

#endif // LZ77_BMP_HPP
