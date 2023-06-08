#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define NUM_GPIO_PINS   21
#define MAX_GPIO_NUMBER 32
#define BUF_SIZE        3
#define PATH_SIZE       20

int main(int argc, char **argv)
{
    int i = 0, index = 0;
    int fd[NUM_GPIO_PINS];
    char path[PATH_SIZE];
    char buf[BUF_SIZE];
    char readBuf[2];

    // Open all GPIO pins
    for (i = 0; i < MAX_GPIO_NUMBER; i++) {
        if (i != 0 && i != 1 && i != 5 && i != 6 &&
            i != 12 && i != 13 && i != 16 && i != 19 &&
            i != 20 && i != 21 && i != 26   ) {
            snprintf(path, sizeof(path), "/dev/raspiGpio%d", i);
            fd[index] = open(path, O_RDWR);
            if (fd[index] < 0) {
                perror("Error opening GPIO pin");
                exit(EXIT_FAILURE);
            }
            index++;
        }
    }

    // Set direction of GPIO pins to input
    printf("Set pins to input\n");
    strncpy(buf, "in", 2);
    buf[2] = '\0';

    for (index = 0; index < NUM_GPIO_PINS; index++) {
        if (write(fd[index], buf, sizeof(buf)) < 0) {
            perror("write, set pin input");
            exit(EXIT_FAILURE);
        }
    }

    // Read logic level of GPIO pins and display them to the terminal
    index = 0;
    for (i = 0; i < MAX_GPIO_NUMBER; i++) {
        if (i != 0 && i != 1 && i != 5 && i != 6 &&
            i != 12 && i != 13 && i != 16 && i != 19 &&
            i != 20 && i != 21 && i != 26) {
            if (read(fd[index], readBuf, 1) < 1) {
                perror("write, set pin input");
                exit(EXIT_FAILURE);
            }
            readBuf[1] = '\0';
            printf("GPIO pin: %d Logic level: %s\n", i, readBuf);
            index++;
        }
    }

    return EXIT_SUCCESS;
}