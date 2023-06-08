#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define NUM_GPIO_PINS   2
#define MAX_GPIO_NUMBER 32
#define BUF_SIZE        3
#define PATH_SIZE       20

int main() {

	int i = 0, index = 0;
    int fd[NUM_GPIO_PINS];
    char path[PATH_SIZE];
    char buf[BUF_SIZE];
    char readBuf[2];
	int pinIn1 = 17, pinIn2 = 22;

	// Open pin GPIO 17
	snprintf(path, sizeof(path), "/dev/raspiGpio%d", pinIn1);
	fd[index] = open(path, O_RDWR);
	if (fd[index] < 0) {
		perror("Error opening GPIO pin");
		exit(EXIT_FAILURE);
	}

	// Open pin GPIO 22
	snprintf(path, sizeof(path), "/dev/raspiGpio%d", pinIn2);
	fd[index] = open(path, O_RDWR);
	if (fd[index] < 0) {
		perror("Error opening GPIO pin");
		exit(EXIT_FAILURE);
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
	for (index = 0; index < NUM_GPIO_PINS; index++) {
		if (read(fd[index], readBuf, 1) < 1) {
			perror("read, set pin input");
			exit(EXIT_FAILURE);
		}
		readBuf[1] = '\0';
		printf("GPIO pin: %d Logic level: %s\n", i, readBuf);
    }

	return EXIT_SUCCESS;
}
