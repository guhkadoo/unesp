#ifndef LZW_TXT_HPP
#define LZW_TXT_HPP

#include "../lzw/lzw.hpp"

class LZW_TXT : public LZW {
private:
    uint8_t* read_text();

public:
    LZW_TXT() = default;
    LZW_TXT(std::string fp) : LZW() { filepath = fp; }
    void compress(int option) override;
    int decompress(int option) override;
};

#endif