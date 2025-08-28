#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUF_SIZE   4096
#define OUT_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)

int copy_data(int fd_in, int fd_out, off_t start, off_t end);
int skip_hole(int fd_out, off_t offset);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <infile> <outfile>\n", argv[0]);
        return 1;
    }

    int fd_in  = open(argv[1], O_RDONLY);
    if (fd_in == -1) { perror("open infile"); return 1; }

    int fd_out = open(argv[2], OUT_FLAGS, 0644);
    if (fd_out == -1) { perror("open outfile"); close(fd_in); return 1; }

    off_t in_size = lseek(fd_in, 0, SEEK_END);
    if (in_size == (off_t)-1) { perror("lseek infile SEEK_END"); close(fd_in); close(fd_out); return 1; }

    for (off_t pos = 0; ; ) {
        off_t data = lseek(fd_in, pos, SEEK_DATA);
        if (data == (off_t)-1) {
            if (errno == ENXIO) break;
            perror("lseek SEEK_DATA");
            close(fd_in); close(fd_out);
            return 1;
        }

        if (data > pos) {
            if (skip_hole(fd_out, data)) { close(fd_in); close(fd_out); return 1; }
        }

        off_t hole = lseek(fd_in, data, SEEK_HOLE);
        if (hole == (off_t)-1) {
            perror("lseek SEEK_HOLE");
            close(fd_in); close(fd_out);
            return 1;
        }

        if (copy_data(fd_in, fd_out, data, hole)) {
            close(fd_in); close(fd_out);
            return 1;
        }

        pos = hole;
    }

    if (ftruncate(fd_out, in_size) == -1) { perror("ftruncate outfile"); close(fd_in); close(fd_out); return 1; }

    if (close(fd_in)  == -1) { perror("close infile");  return 1; }
    if (close(fd_out) == -1) { perror("close outfile"); return 1; }
    return 0;
}

int copy_data(int fd_in, int fd_out, off_t start, off_t end) {
    char buf[BUF_SIZE];
    off_t left = end - start;

    if (lseek(fd_in, start, SEEK_SET) == (off_t)-1) { perror("lseek fd_in"); return 1; }

    while (left > 0) {
        size_t chunk = (left > BUF_SIZE) ? BUF_SIZE : (size_t)left;
        ssize_t n = read(fd_in, buf, chunk);
        if (n <= 0) { perror("read"); return 1; }

        size_t off = 0;
        while (off < (size_t)n) {
            ssize_t w = write(fd_out, buf + off, (size_t)n - off);
            if (w <= 0) { perror("write fd_out"); return 1; }
            off += (size_t)w;
        }
        left -= n;
    }
    return 0;
}

int skip_hole(int fd_out, off_t offset) {
    if (lseek(fd_out, offset, SEEK_SET) == (off_t)-1) { perror("lseek fd_out"); return 1; }
    return 0;
}