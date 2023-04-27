#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"

/*
    usage: ./huffmaning [-D or --decompress | -C or --compress] [-t or --type] <input file> <output file>
    -D or --decompress: decompress the input file
    -C or --compress: compress the input file
    -t or --type: type compression or decompression
        -t 0: compress or decompress using the huffman algorithm per character
        -t 1: compress or decompress using the huffman algorithm per word
        -t 2: compress or decompress using the huffman algorithm per token
    <input file>: file to be compressed or decompressed
    <output file>: file to be written the result
*/
int main(int argc, char *argv[]) {
    int option = -1;
    int type = -1;
    char *input_file;
    char *output_file;

    if (argc < 4) {
        printf("Usage: ./huffmaning [-D or --decompress | -C or --compress] [-t or --type] <input_file> <output_file>\n");
        return 0;
    }

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-D") == 0 || strcmp(argv[i], "--decompress") == 0) {
            option = 0;
        } else if (strcmp(argv[i], "-C") == 0 || strcmp(argv[i], "--compress") == 0) {
            option = 1;
        } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--type") == 0) {
            i++;
            if (i < argc) {
                type = atoi(argv[i]);
            } else {
                printf("Error: missing argument for -t or --type option\n");
                return 0;
            }
        } else if (i == argc - 2) {
            input_file = argv[i];
        } else if (i == argc - 1) {
            output_file = argv[i];
        }
    }

    if (option == -1 || type == -1 || input_file == NULL || output_file == NULL) {
        printf("Error: missing required options or arguments\n");
        return 0;
    }

    if (option == 0) {
        if (type == 0) {
            // Decompress per character
            huffman_decode_file_per_char(input_file, output_file);
        } else if (type == 1) {
            // Decompress per word
            huffman_decode_file_per_word(input_file, output_file);
        }
    } else if (option == 1) {
        if (type == 0) {
            // Compress per character
            huffman_encode_file_per_char(input_file, output_file);
        } else if (type == 1) {
            // Compress per word
            huffman_encode_file_per_word(input_file, output_file);
        }
    }

    return 0;
}