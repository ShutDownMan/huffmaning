#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"

#define INVALID_ARGUMENTS -1
#define INVALID_OPTION -2
#define INVALID_TYPE -3

#define OPTION_COMPRESS 1
#define OPTION_DECOMPRESS 0
#define TYPE_CHAR 0
#define TYPE_WORD 1
#define TYPE_TOKEN 2

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
            option = OPTION_DECOMPRESS;
        } else if (strcmp(argv[i], "-C") == 0 || strcmp(argv[i], "--compress") == 0) {
            option = OPTION_COMPRESS;
        } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--type") == 0) {
            i++;
            if (i < argc) {
                type = atoi(argv[i]);
            } else {
                printf("Error: missing argument for -t or --type option\n");
                return -1;
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

    switch (option) {
        case OPTION_DECOMPRESS:
            switch (type) {
                case TYPE_CHAR:
                    // Decompress per character
                    huffman_decode_file_per_char(input_file, output_file);
                    break;
                case TYPE_WORD:
                    // Decompress per word
                    huffman_decode_file_per_word(input_file, output_file);
                    break;
                case TYPE_TOKEN:
                    // Decompress per token
                    // huffman_decode_file_per_token(input_file, output_file);
                    break;
                default:
                    printf("Error: invalid type\n");
                    return INVALID_TYPE;
            }
            break;
        case OPTION_COMPRESS:
            switch (type) {
                case TYPE_CHAR:
                    // Compress per character
                    huffman_encode_file_per_char(input_file, output_file);
                    break;
                case TYPE_WORD:
                    // Compress per word
                    huffman_encode_file_per_word(input_file, output_file);
                    break;
                case TYPE_TOKEN:
                    // Compress per token
                    // huffman_encode_file_per_token(input_file, output_file);
                    break;
                default:
                    printf("Error: invalid type\n");
                    return INVALID_TYPE;
            }
            break;
        default:
            printf("Error: invalid option\n");
            return INVALID_OPTION;
    }

    return 0;
}