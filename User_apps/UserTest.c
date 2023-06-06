#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {

	int f = open("/dev/GPIO_Driver", O_RDWR);

	if(f < 0) {
	    printf("Could not open dev file\n");
	    return 1;
	}

	while(1)
	{
		write(f, "22,1", 4);
		usleep(500000);
		write(f, "22,0", 4);
		usleep(500000);
	}


	return 0;
}
