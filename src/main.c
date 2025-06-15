#include <stdio.h>
#include <string.h>
#include "../include/compress.h"
#include "../include/decompress.h"

void usage() {
    printf("Usage:\n");
    printf("  ./data_compressor compress <method> <input_file> <output_file>\n");
    printf("  ./data_compressor decompress <method> <compressed_file>\n");
    printf("Supported methods: zstd, paq8px, cmix, zpaq\n");
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        usage();
        return 1;
    }

    const char *action = argv[1];
    const char *method = argv[2];

    if (strcmp(action, "compress") == 0 && argc == 5) {
        compressFile(method, argv[3], argv[4]);
    } else if (strcmp(action, "decompress") == 0 && argc == 4) {
        decompressFile(method, argv[3]);
    } else {
        usage();
        return 1;
    }

    return 0;
}
