#ifndef LZ77_HPP
#define LZ77_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <cstdio>

#define EXIT_WITH_ERROR(msg) (fprintf(stderr, "%s\n", msg), exit(1))

class LZ77
{
protected:
    std::string filepath;
    static const int WINDOW_SIZE = 4096;
    static const int MAX_LENGTH = 63;

    class BitWriter
    {
    private:
        FILE* file;
        uint32_t buffer = 0;
        int bitCount = 0;
    public:
        BitWriter(FILE* f) : file(f) {}
        void writeBit(bool bit);
        void writeBits(uint32_t value, int bits);
        void flush();
        void finalize();
    };

    class BitReader
    {
    private:
        FILE* file;
        uint32_t buffer = 0;
        int bitCount = 0;
    public:
        BitReader(FILE* f) : file(f) {};
        bool readBit();
        uint32_t readBits(int bits);
    };

    void internal_compress(uint8_t* data, size_t data_size, void (*write_header)(FILE*, void*) = nullptr, void* filetype
            = nullptr);
    static std::string get_compressed_filepath(const std::string& filepath);
    static std::string decompressed_filepath_from_compressed(const std::string& compressed_path);

public:
    LZ77() : filepath("") {}
    LZ77(std::string fp) : filepath(fp) {}
    int set_filepath(const std::string& path);
    virtual void compress(int option) = 0;
    int decompress(int option, void (*write_header)(FILE*, void*) = nullptr, void (*read_header)(FILE*, void*) =
            nullptr, size_t (*get_pos)(void*) = nullptr, void* filetype = nullptr);
};

#endif //LZ77_HPP
