#include "../include/lz77/lz.hpp"
#include <cstring>
#include <unistd.h>

void LZ77::BitWriter::writeBit(bool bit)
{
    buffer = (buffer << 1) | (bit ? 1 : 0);
    bitCount++;
    if(bitCount == 8) flush();
}

void LZ77::BitWriter::writeBits(uint32_t value, int bits)
{
    for(int i=bits-1; i>=0; i--)
        writeBit((value >> i) & 1);
}

void LZ77::BitWriter::flush()
{
    while(bitCount >= 8)
    {
        uint8_t byte = (buffer >> (bitCount - 8)) & 0xFF;
        fwrite(&byte, 1, 1, file);
        bitCount -= 8;
        buffer &= (1 << bitCount) - 1;
    }
}

void LZ77::BitWriter::finalize()
{
    if(bitCount > 0)
    {
        buffer <<= (8 - bitCount);
        fwrite(&buffer, 1, 1, file);
    }
}

bool LZ77::BitReader::readBit()
{
    if(bitCount == 0)
    {
        uint8_t byte;
        if(fread(&byte, 1, 1, file) != 1) return false;
        buffer = byte;
        bitCount += 8;
    }
    bool bit = (buffer >> (bitCount - 1)) & 1;
    bitCount--;
    return bit;
}

uint32_t LZ77::BitReader::readBits(int bits)
{
    uint32_t value = 0;
    for(int i=0; i<bits; i++)
        value = (value << 1) | (readBit() ? 1 : 0);
    return value;
}

int LZ77::set_filepath(const std::string& path) {
    filepath = path;
    FILE* file = fopen(filepath.c_str(), "rb");
    if (!file) return -1;
    fclose(file);
    return 0;
}

std::string LZ77::get_compressed_filepath(const std::string& filepath) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) EXIT_WITH_ERROR("getcwd() error");
    std::string filename = cwd;
    filename.append("/data/output/");
    size_t last_slash = filepath.find_last_of("/\\");
    std::string filename_w_ext = filepath.substr(last_slash + 1);
    size_t dot = filename_w_ext.find_last_of(".");
    std::string base = filename_w_ext.substr(0, dot);
    std::string ext = filename_w_ext.substr(dot);
    filename.append(base + "_lz77" + ext + ".GK");
    return filename;
}

std::string LZ77::decompressed_filepath_from_compressed(const std::string& compressed_path) {
    size_t pos = compressed_path.find(".GK");
    if (pos != std::string::npos) {
        std::string base = compressed_path.substr(0, pos);
        size_t lz77_pos = base.find("_lz77");
        if (lz77_pos != std::string::npos) {
            return base.substr(0, lz77_pos) + "decompressed" + base.substr(lz77_pos + 5);
        }
    }
    return compressed_path;
}

void LZ77::internal_compress(uint8_t* data, size_t data_size, void (*write_header)(FILE*, void*), void* filetype)
{
    std::string outputFile = get_compressed_filepath(filepath);
    FILE* out = fopen(outputFile.c_str(), "wb");
    if (!out) return;

    // HEADER
    fwrite("GK", 1, 2, out);
    uint8_t version = 1;
    fwrite(&version, 1, 1, out);
    uint32_t origSize = data_size;
    fwrite(&origSize, 4, 1, out);
    uint8_t reserved = 0;
    fwrite(&reserved, 1, 1, out);

    if (write_header && filetype)
        write_header(out, filetype);

    BitWriter writer(out);
    size_t pos = 0;
    while (pos < data_size) {
        int bestOffset = 0, bestLength = 0;
        int windowStart = std::max(0, static_cast<int>(pos) - WINDOW_SIZE);

        for (int i = windowStart; i < static_cast<int>(pos); ++i) {
            int length = 0;
            while (length < MAX_LENGTH && pos + length < data_size &&
                   data[i + length] == data[pos + length]) {
                ++length;
            }
            if (length > bestLength && length >= 3) {
                bestLength = length;
                bestOffset = pos - i;
            }
        }

        if (bestLength >= 3) {
            writer.writeBit(1);
            writer.writeBits(bestOffset, 12);
            writer.writeBits(bestLength, 6);
            uint8_t nextChar = (pos + bestLength < data_size) ? data[pos + bestLength] : 0;
            writer.writeBits(nextChar, 8);
            pos += bestLength + 1;
        } else {
            writer.writeBit(0);
            writer.writeBits(data[pos], 8);
            pos += 1;
        }
    }
    writer.finalize();
    fclose(out);
}

int LZ77::decompress(int option, void (*write_header)(FILE*, void*), void (*read_header)(FILE*, void*), size_t (*get_pos)(void*), void* filetype) {
    FILE* in = fopen(filepath.c_str(), "rb");
    if (!in) return -1;

    char magic[2];
    fread(magic, 1, 2, in);
    if (magic[0] != 'G' || magic[1] != 'K') {
        fclose(in);
        return -1;
    }
    uint8_t version;
    fread(&version, 1, 1, in);
    uint32_t origSize;
    fread(&origSize, 4, 1, in);
    fseek(in, 1, SEEK_CUR); // Skip reserved byte

    size_t headerSize = 0;
    if (read_header && filetype) {
        long startPos = ftell(in);
        read_header(in, filetype);
        headerSize = ftell(in) - startPos;
    }

    BitReader reader(in);
    std::vector<uint8_t> buffer;
    buffer.reserve(origSize);

    while (buffer.size() < origSize) {
        bool flag = reader.readBit();
        if (flag) {
            uint32_t offset = reader.readBits(12);
            uint32_t length = reader.readBits(6);
            uint8_t nextChar = reader.readBits(8);
            size_t start = buffer.size() - offset;
            for (int i = 0; i < length; ++i) {
                buffer.push_back(buffer[start + i]);
            }
            buffer.push_back(nextChar);
        } else {
            uint8_t ch = reader.readBits(8);
            buffer.push_back(ch);
        }
    }

    std::string outputFile = decompressed_filepath_from_compressed(filepath);
    FILE* out = fopen(outputFile.c_str(), "wb");
    if (!out) {
        fclose(in);
        return -1;
    }
    if (write_header && filetype) {
        write_header(out, filetype);
    }
    fwrite(buffer.data(), 1, buffer.size(), out);
    fclose(out);
    fclose(in);
    return 0;
}
