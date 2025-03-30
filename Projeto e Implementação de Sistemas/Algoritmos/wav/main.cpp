#include "wav.hpp"
#include <cstdio>

int main(int argc, char* argv[])
{
    wav wav_file;
    if(wav_file.read_wav("file_example.wav") != -1)
        printf("file_example read\n");

    wav_file.print_wav(); 
}
