#ifndef LZW_HPP
#define LZW_HPP

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <cstdio>

#define RESET_CODE 4095 // Fixed reset code
#define RESET_CODE_WIDTH 12 // Fixed width for RESET_CODE

#define EXIT_WITH_ERROR(msg) (fprintf(stderr, "%s\n", msg), exit(1))

class LZW {
protected:
    std::string filepath;
    static const size_t MAX_DICT_SIZE = 100; // Max dictionary entries
    static const size_t INITIAL_DICT_SIZE = 256; // ASCII characters
    static const size_t INITIAL_CODE_WIDTH = 9; // Start with 9-bit codes

    // Dictionary for compression (string to code)
    std::map<std::string, uint16_t> compress_dict;
    // Dictionary for decompression (code to string)
    std::map<uint16_t, std::string> decompress_dict;

    class BitWriter {
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

    class BitReader {
    private:
        FILE* file;
        uint32_t buffer = 0;
        int bitCount = 0;
        int currentCodeWidth = INITIAL_CODE_WIDTH;
    public:
        BitReader(FILE* f, int initialWidth) : file(f), currentCodeWidth(initialWidth) {}
        bool readBit();
        uint32_t readBits(int bits);
        void setCodeWidth(int width) { currentCodeWidth = width; }
    };

    std::string get_compressed_filepath(int option);
    std::string get_decompressed_filepath(int option);
    std::string decompressed_filepath_from_compressed(const std::string& compressed_path);
    std::vector<std::pair<uint16_t, int>> encode(const uint8_t* data, size_t size);
    std::vector<uint8_t> decode(const std::vector<std::pair<uint16_t, int>>& codes);
    void internal_compress(int option, const std::vector<std::pair<uint16_t, int>>& codes, void (*write_header)(FILE*, void*) = nullptr, void* filetype = nullptr);
    int internal_decompress(int option, void (*write_header)(FILE*, void*) = nullptr, void (*read_header)(FILE*, void*) = nullptr, size_t (*get_pos)(void*) = nullptr, void* filetype = nullptr);

public:
    LZW() = default;
    virtual ~LZW() = default;
    int set_filepath(const std::string& str);
    std::string get_filepath();
    virtual void compress(int option) = 0;
    virtual int decompress(int option) = 0;
};

#endif