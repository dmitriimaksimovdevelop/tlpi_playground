#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE   4096
#define OUT_FLAGS (O_WRONLY | O_CREAT | O_APPEND)

int main(int argc, char *argv[]) {
    if (argc == 2) {
        int fd_out = open(argv[1], OUT_FLAGS, 0644);
        int fd_dup = fcntl(fd_out, F_DUPFD, 6);
        if (fd_out == -1) { perror("open outfile"); return 1; }
        if (fd_dup == -1) { perror("dup outfile"); return 1; }

        ssize_t written = write(3, "bb\n", 3);
        ssize_t writtendup = write(6, "aa\n", 3);

        if (written == -1) { perror("write outfile"); close(fd_out); return 1; }
        if (writtendup == -1) { perror("write outfile"); close(fd_dup); return 1; }

        close(fd_out);
        close(fd_dup);
    } else {
        fprintf(stderr, "Usage: %s [FILE]\n", argv[0]);
    }
    return 0;
}