#include "wav.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    wav wav_file;
    if(read_wav("file_example.wav", &wav_file) != -1)
        printf("file_example read\n");

    print_wav(&wav_file); 
}
