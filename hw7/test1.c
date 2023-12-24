#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int fd;
    int max_fds = 0;

    // Keep trying to open files until an error occurs
    while (1) {
        fd = open("/dev/null", O_RDONLY);

        if (fd == -1) {
            if (errno == EMFILE) {
                // EMFILE indicates that the process has reached the maximum
                // number of file descriptors it can open
                break;
            } else {
                perror("open");
                exit(EXIT_FAILURE);
            }
        } else {
            // Increment the count of successfully opened file descriptors
            max_fds++;
        }
    }

    printf("Available file descriptors: %d\n", max_fds);

    return 0;
}

