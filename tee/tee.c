#define _FILE_OFFSET_BITS 64
#define ERR_NO_FILE "You must add file for output with -f\n"
#define BUF_SIZE 4096

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int def_tee(int file_flags, int ap, int argc, char *argv[]);
int def_tee_buf(int file_flags, int ap, int argc, char *argv[]);

int main(int argc, char *argv[]) {
    int ap;
    int file_flags = O_WRONLY | O_CREAT;
    if (argc < 3) {
        fprintf (stderr,ERR_NO_FILE);
        return 1;
    }
    for (ap = 1 ; ap < argc ; ap++) {
        if (argv[ap][0] == '-') {
            switch (argv[ap][1]) {
                case 'f':
                    file_flags |= O_TRUNC;
                    return def_tee_buf(file_flags, ap, argc, argv);
                case 'a':
                    file_flags |= O_APPEND;
                    return def_tee_buf(file_flags, ap, argc, argv);
                default:
                    fprintf (stderr, "Unknown option: %s\n", argv[ap]);
                    return 1;
            }
        }
    }
    return 0;
}

int def_tee(int file_flags, int ap, int argc, char *argv[]) {
    int c;
    if (ap + 1 < argc) {
        char *filename = argv[++ap];
        int fd = open(filename, file_flags, 0644 );
        if (fd == -1) { perror("open"); return 1; }
        while ((c = getchar()) != EOF) {
            if (putchar(c) == EOF) { perror("putchar"); return 1; }
            if (write(fd, &c, 1) == -1) { perror("write"); return 1; }
        }
        if (close(fd) == -1) { perror("close"); return 1; }
    } else {
        fprintf (stderr,ERR_NO_FILE);
        return 1;
    }
}

int def_tee_buf(int file_flags, int ap, int argc, char *argv[]) {
    char buf[BUF_SIZE];
    ssize_t numRead;

    if (ap + 1 < argc) {
        char *filename = argv[++ap];
        int fd = open(filename, file_flags, 0644);
        if (fd == -1) { perror("open"); return 1; }

        while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
            if (write(STDOUT_FILENO, buf, numRead) != numRead) {
                perror("write stdout");
                close(fd);
                return 1;
            }
            ssize_t totalWritten = 0;
            while (totalWritten < numRead) {
                ssize_t written = write(fd, buf + totalWritten, numRead - totalWritten);
                if (written == -1) { perror("write"); return 1; }
                totalWritten += written;
            }
        }
        if (numRead == -1) { perror("read"); close(fd); return 1; }

        if (close(fd) == -1) { perror("close"); return 1; }
    } else {
        fprintf(stderr, ERR_NO_FILE);
        return 1;
    }
    return 0;
}