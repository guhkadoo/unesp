#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

const int WINDOW_SIZE = 4096;
const int MAX_LENGTH = 63;

class BitWriter
{
private:
    std::ofstream& out;
    uint32_t buffer = 0;
    int bitCount = 0;
public:
    BitWriter(std::ofstream& stream) : out(stream) {}

    void writeBit(bool bit)
    {
        buffer = (buffer << 1) | (bit ? 1 : 0);
        bitCount++;
        if(bitCount == 8) flush();
    }

    void writeBits(uint32_t value, int bits)
    {
        for(int i=bits-1; i>=0; --i)
            writeBit((value >> i) & 1);
    }

    void flush()
    {
        while(bitCount >= 8)
        {
            uint8_t byte = (buffer >> (bitCount - 8)) & 0xFF;
            out.write(reinterpret_cast<char*>(&byte), 1);
            bitCount -= 8;
            buffer &= (1 << bitCount) - 1;
        }
    }

    void finalize()
    {
        if(bitCount > 0)
        {
            buffer <<= (8 - bitCount);

        }
    }
};
