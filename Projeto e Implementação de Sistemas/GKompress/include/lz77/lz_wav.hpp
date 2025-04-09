#ifndef LZ77_WAV_HPP
#define LZ77_WAV_HPP

#include "../lz77/lz.hpp"
#include "../formats/wav.hpp"
#include <string>

class LZ77WAV : public LZ77 {
private:
    wav wav_file;
public:
    LZ77WAV() : LZ77() {}
    LZ77WAV(std::string fp) : LZ77(fp) {}
    void compress(int option) override;
};

#endif // LZ77_WAV_HPP
