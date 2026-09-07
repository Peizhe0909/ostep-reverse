#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// read arbitrary long line (dynamic buffer), returns length or -1 on EOF
int read_line(FILE *fp, char **out_buf, size_t *out_len) {
    size_t cap = 256;
    size_t len = 0;
    char *buf = malloc(cap);
    if (buf == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (len + 2 > cap) {
            cap *= 2;
            char *nb = realloc(buf, cap);
            if (nb == NULL) {
                free(buf);
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
            buf = nb;
        }
        buf[len++] = (char)c;
        if (c == '\n') break;
    }
    if (len == 0 && c == EOF) {
        free(buf);
        return -1;
    }
    buf[len] = '\0';
    *out_buf = buf;
    *out_len = len;
    return (int)len;
}

// grep one stream: if search term found in line, print it
void grep_stream(FILE *fp, const char *term) {
    char *buf = NULL;
    size_t len = 0;
    int nread;
    while ((nread = read_line(fp, &buf, &len)) != -1) {
        if (strstr(buf, term) != NULL) {
            printf("%s", buf);
        }
        free(buf);
        buf = NULL;
    }
}

int main(int argc, char *argv[]) {
    // no args at all: error
    if (argc < 2) {
        fprintf(stderr, "my-grep: searchterm [file ...]\n");
        return 1;
    }

    const char *term = argv[1];

    if (argc == 2) {
        // only search term: read stdin
        grep_stream(stdin, term);
    } else {
        // term + files: process each file
        for (int i = 2; i < argc; i++) {
            FILE *fp = fopen(argv[i], "r");
            if (fp == NULL) {
                fprintf(stderr, "my-grep: cannot open file\n");
                return 1;
            }
            grep_stream(fp, term);
            fclose(fp);
        }
    }

    return 0;
}
