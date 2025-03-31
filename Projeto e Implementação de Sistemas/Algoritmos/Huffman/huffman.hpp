#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <array>
#include <string>

#define EXIT_WITH_ERROR(msg) (fprintf(stderr, "%s\n", msg), exit(1))

#define B "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte) \
    ((byte) & 0x80 ? '1' : '0'), \
    ((byte) & 0x40 ? '1' : '0'), \
    ((byte) & 0x20 ? '1' : '0'), \
    ((byte) & 0x10 ? '1' : '0'), \
    ((byte) & 0x08 ? '1' : '0'), \
    ((byte) & 0x04 ? '1' : '0'), \
    ((byte) & 0x02 ? '1' : '0'), \
    ((byte) & 0x01 ? '1' : '0')

class Huffman {
public:
    Huffman() : filepath("") {};
    Huffman(std::string fp) : filepath(fp) {};
    void set_filepath(std::string str);
    void decompress(void (*write_header)(FILE*, void*) = nullptr, void* filetype = nullptr, size_t pos = 0);
    virtual void compress() = 0;

protected:
    static const size_t SIZE = 256;
    std::array<uint32_t, SIZE> duplicates = {};
    std::string filepath;

    class Node {
    public:
        uint8_t byte;
        uint32_t duplicates;
        Node* left;
        Node* right;
        Node* next;

        Node(uint8_t b, uint32_t dup) : byte(b), duplicates(dup), left(nullptr), right(nullptr), next(nullptr) {}
    };
    class List {
    public:
        Node* front;
        uint32_t size;

        List() : front(nullptr), size(0) {}
        void insert_sorted(Node* node);
        Node* remove_front();
        void fill(std::array<uint32_t, Huffman::SIZE>& duplicates);
        void print();
        // We don't need a destructor for the list, because the 
        // destructor of the tree already frees the memory of the list.
    }; 
    class HuffmanTree {
    private:
        void print_with_argument(Node* root);
        int height_with_argument(Node* root); 
    public:
        Node* root;
        void build(List& list);
        void delete_tree(Node* node);
        void print();
        int height();
        ~HuffmanTree();
    };
    class Dictionary {
    public:
        char** dictionary;
        char*& operator[](size_t index);
        void fill(Node* tree_root, char* code, size_t columns);
        void print();
        void build(size_t j);
        ~Dictionary();
    };
    HuffmanTree tree;
    Dictionary dictionary;

    char *decode(char *code, struct Node *tree);
    void print_duplicates();
    void count_duplicates(uint8_t* data, size_t size);
    
    void internal_compress(char* code, void (*write_header)(FILE*, void*) = nullptr, void* filetype = nullptr);
    char* encode(uint8_t* data, size_t data_size);
    List list;
};

#endif //HUFFMAN_HPP
