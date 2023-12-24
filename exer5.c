#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int fd1, fd2;
    if ((fd1 = open("foo.bar", O_CREAT | O_RDONLY, 0644)) == -1) {
        perror("open foo.bar");
        return 1;
    }
    if ((fd2 = open("result", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        perror("open result");
        close(fd1);
        return 1;
    }
    if (close(0) == -1) {
        perror("close stdin");
        close(fd1);
        close(fd2);
        return 1;
    }
    if (dup(fd1) == -1) {
        perror("dup fd1 to stdin");
        close(fd1);
        close(fd2);
        return 1;
    }
    close(fd1);
    if (close(1) == -1) {
        perror("close stdout");
        close(fd2);
        return 1;
    }
    if (dup(fd2) == -1) {
        perror("dup fd2 to stdout");
        close(fd2);
        return 1;
    }

    close(fd2);
    execlp("./a.out", "a.out", (char *)NULL);
    perror("execlp");
    return 1;
}

