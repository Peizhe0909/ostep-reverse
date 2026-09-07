#include <stdio.h>
#include <stdlib.h>

// print one file to stdout
void print_file(const char *fname) {
    FILE *fp = fopen(fname, "r");
    if (fp == NULL) {
        fprintf(stderr, "my-cat: cannot open file\n");
        exit(1);
    }
    char buf[4096];
    // read chunks, handle any line length
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        printf("%s", buf);
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    // no files: just exit 0 (per assignment spec)
    if (argc == 1) {
        return 0;
    }
    // print each file in order
    for (int i = 1; i < argc; i++) {
        print_file(argv[i]);
    }
    return 0;
}
