#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <array>
#include <vector>
#include <string>
#include <bitset>
#include <map>
#include <iostream>
#include <algorithm>

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
    int set_filepath(std::string str);
    std::string get_filepath();
    int decompress(int option, void (*write_header)(FILE*, void*) = nullptr, void (*read_header)(FILE*, void*) = nullptr, size_t (*get_pos)(void*) = nullptr, void* filetype = nullptr);
    virtual void compress(int option) = 0;

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

        int is_leaf();
        Node(uint8_t b, uint32_t dup) : byte(b), duplicates(dup), left(nullptr), right(nullptr), next(nullptr) {}
    };
    class List {
    public:
        Node* front = nullptr;
        uint32_t size;

        List() : front(nullptr), size(0) {}
        void insert_sorted(Node* node);
        Node* remove_front();
        void fill(std::array<uint32_t, Huffman::SIZE>& duplicates);
        void print();
        void clear();
        // We don't need a destructor for the list, because the 
        // destructor of the tree already frees the memory of the list.
    }; 
    class HuffmanTree {
    private:
        void print_with_argument(Node* root);
        int height_with_argument(Node* root); 
    public:
        Node* root = nullptr;
        void build(List& list);
        void delete_tree(Node* node);
        void print();
        int height();
        void clear(Node*);
        void write(FILE* file, Node* node);
        Node* read(FILE* file);

        ~HuffmanTree();
    };
    class Dictionary {
    public:
        char** dictionary = nullptr;
        std::vector<std::pair<uint8_t, size_t>> sorted_symbols; // {symbol, code length}

        char*& operator[](size_t index);
        void fill(Node* tree_root, char* code, size_t columns, int option);
        void generate_canonical_codes();
        std::map<std::string, uint8_t> get_reverse_map() const;
        void print();
        void build(size_t j); 
        void write(FILE* file);
        void read(FILE* file);
        void clear();
        ~Dictionary();
    };
    HuffmanTree tree;
    Dictionary dictionary;

    void clean_for_next_iteration();
    char *decode(char *code, struct Node *tree);
    void print_duplicates();
    void count_duplicates(uint8_t* data, size_t size);
    
    void internal_compress(int option, char* code, void (*write_header)(FILE*, void*) = nullptr, void* filetype = nullptr);
    char* encode(uint8_t* data, size_t data_size);
    List list;
};

#endif //HUFFMAN_HPP
