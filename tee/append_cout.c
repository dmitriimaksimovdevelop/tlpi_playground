#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE   4096
#define OUT_FLAGS (O_WRONLY | O_CREAT | O_APPEND)

int main(int argc, char *argv[]) {
    int i;

    if (argc > 4 || argc < 3) {
        fprintf(stderr, "Usage: %s <outfile> <num-bytes> A\n", argv[0]);
        return 1;
    }

    if (argc == 3) {
        int fd_out = open(argv[1], OUT_FLAGS, 0644);
        if (fd_out == -1) { perror("open outfile"); return 1; }

        int count = atoi(argv[2]);
        for (i= 0; i < count; i++) {
            ssize_t written = write(fd_out, "b", 1);
            if (written == -1) { perror("write outfile"); close(fd_out); return 1; }
        }
        close(fd_out);
    }

    if (argc == 4) {
        int fd_out = open(argv[1], O_WRONLY | O_CREAT, 0644);
        if (fd_out == -1) { perror("open outfile"); return 1; }


        int count = atoi(argv[2]);
        for (i= 0; i < count; i++) {
            off_t in_size = lseek(fd_out, 0, SEEK_END);
            if (in_size == (off_t)-1) { perror("lseek outfile SEEK_END"); close(fd_out); return 1; }
            ssize_t written = write(fd_out, "b", 1);
            if (written == -1) { perror("write outfile"); close(fd_out); return 1; }
        }
        close(fd_out);
    }
    return 0;
}