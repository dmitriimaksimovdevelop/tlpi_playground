#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE   4096
#define OUT_FLAGS (O_WRONLY | O_CREAT | O_APPEND)

int main(int argc, char *argv[]) {
    if (argc == 2) {
        int fd_out = open(argv[1], OUT_FLAGS, 0644);
        int fd1 = open(argv[1], O_RDWR| O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        int fd2 = dup(fd1);
        int fd3 = open(argv[1], O_RDWR);
        write(fd1, "Hello,", 6);
        write(fd2, " world,", 6);
        lseek(fd2, 0, SEEK_SET);
        write(fd1, "HELLO,", 6);
        write(fd3, "Gidday,", 6);
    }
    return 0;
}
