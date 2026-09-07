#include <stdio.h>
#include <stdlib.h>

// RLE decompress: read (4-byte int count, 1-byte char) from fp, write count chars to stdout
void decompress_stream(FILE *fp) {
    int count;
    char ch;
    while (fread(&count, sizeof(int), 1, fp) == 1) {
        if (fread(&ch, sizeof(char), 1, fp) != 1) {
            break;
        }
        for (int i = 0; i < count; i++) {
            fputc(ch, stdout);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "my-unzip: file1 [file2 ...]\n");
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "rb");
        if (fp == NULL) {
            fprintf(stderr, "my-unzip: cannot open file\n");
            return 1;
        }
        decompress_stream(fp);
        fclose(fp);
    }
    return 0;
}
