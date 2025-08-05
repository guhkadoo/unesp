#ifndef LZW_WAV_HPP
#define LZW_WAV_HPP

#include "../lzw/lzw.hpp"
#include "../formats/wav.hpp"

class LZW_WAV : public LZW {
private:
    wav wav_file;

public:
    LZW_WAV() = default;
    LZW_WAV(std::string fp) : LZW() { filepath = fp; }
    void compress(int option) override;
    int decompress(int option) override;
};

#endif