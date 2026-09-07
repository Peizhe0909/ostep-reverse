#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// node for line
typedef struct Node {
    char *line;
    struct Node *next;
} Node;

// add node to list (head insert)
Node* add_node(Node *head, char *line) {
    Node *n = malloc(sizeof(Node));
    if (n == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    n->line = line;
    n->next = head;
    return n;
}

// free list
void free_list(Node *head) {
    Node *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp->line);
        free(tmp);
    }
}

// check same file (simple path compare)
int same_file(const char *f1, const char *f2) {
    if (f1 == NULL || f2 == NULL) return 0;
    return strcmp(f1, f2) == 0;
}

// read one line from fp, dynamic buffer, returns length or -1 on EOF
int read_line(FILE *fp, char **out_buf, size_t *out_len) {
    size_t cap = 128;
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

int main(int argc, char *argv[]) {
    FILE *in = stdin;
    FILE *out = stdout;
    char *in_name = NULL;
    char *out_name = NULL;

    // arg check
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        return 1;
    }

    if (argc >= 2) {
        in_name = argv[1];
        in = fopen(in_name, "r");
        if (in == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", in_name);
            return 1;
        }
    }

    if (argc == 3) {
        out_name = argv[2];
        // same file check
        if (same_file(in_name, out_name)) {
            fprintf(stderr, "Input and output file must differ\n");
            fclose(in);
            return 1;
        }
        out = fopen(out_name, "w");
        if (out == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", out_name);
            fclose(in);
            return 1;
        }
    }

    // read lines into list
    Node *head = NULL;
    char *buf = NULL;
    size_t len = 0;
    int nread;

    while ((nread = read_line(in, &buf, &len)) != -1) {
        // buf is already the line with \0, just add to list
        head = add_node(head, buf);
        buf = NULL; // reset for next line
    }

    // print reversed (list is already reversed due to head insert)
    Node *cur = head;
    while (cur != NULL) {
        fprintf(out, "%s", cur->line);
        cur = cur->next;
    }

    // cleanup
    free_list(head);
    if (in != stdin) fclose(in);
    if (out != stdout) fclose(out);

    return 0;
}
