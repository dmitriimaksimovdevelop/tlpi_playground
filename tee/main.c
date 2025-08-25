#define _FILE_OFFSET_BITS 64
#define ERR_NO_FILE "You must add file for output with -f\n"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int c;
    int ap;
    if (argc <3) {
        printf (ERR_NO_FILE);
        return 1;
    }
    for (ap = 1 ; ap < argc ; ap++) {
        if (argv[ap][0] == '-') {
            switch (argv[ap][1]) {
                case 'f':
                    if (ap + 1 < argc) {
                        char *filename = argv[++ap];
                        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644 );
                        if (fd == -1) { perror("open"); return 1; }
                        while ((c = getchar()) != EOF) {
                            if (putchar(c) == -1) { perror("put"); return 1; }
                            if (write(fd, &c, 1) == -1) { perror("write"); return 1; }
                        }
                        if (close(fd) == -1) { perror("close"); return 1; }
                    } else {printf (ERR_NO_FILE);}
                    return 0;
                default:
                    printf (ERR_NO_FILE);
            }
        }
    }
    return 0;
}