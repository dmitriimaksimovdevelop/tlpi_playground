#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE   4096
#define OUT_FLAGS (O_WRONLY | O_CREAT | O_APPEND)

int main(int argc, char *argv[]) {
    char buf[]= "hello world\n";

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <outfile>\n", argv[0]);
        return 1;
    }

    int fd_out = open(argv[1], OUT_FLAGS, 0644);
    if (fd_out == -1) { perror("open outfile"); close(fd_out); return 1; }

    off_t in_size = lseek(fd_out, 0, SEEK_SET);
    if (in_size == (off_t)-1) { perror("lseek infile SEEK_END"); close(fd_out); return 1; }

    ssize_t written = write(fd_out, buf, strlen(buf));
    if (written == -1) { perror("write infile"); close(fd_out); return 1; }

    return 0;
}