#include <stdio.h>
#include <stdlib.h>

// RLE compress: read from fp, write (4-byte int count, 1-byte char) runs to stdout
void compress_stream(FILE *fp) {
    int c;
    int prev = EOF;
    int count = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (prev == EOF) {
            prev = c;
            count = 1;
        } else if (c == prev) {
            count++;
        } else {
            // flush previous run
            fwrite(&count, sizeof(int), 1, stdout);
            char ch = (char)prev;
            fwrite(&ch, sizeof(char), 1, stdout);
            prev = c;
            count = 1;
        }
    }
    // flush last run if any
    if (prev != EOF) {
        fwrite(&count, sizeof(int), 1, stdout);
        char ch = (char)prev;
        fwrite(&ch, sizeof(char), 1, stdout);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "my-zip: file1 [file2 ...]\n");
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "my-zip: cannot open file\n");
            return 1;
        }
        compress_stream(fp);
        fclose(fp);
    }
    return 0;
}
