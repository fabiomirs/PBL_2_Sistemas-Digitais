
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

#define DEVICE "/dev/gpu123"

 /* Prototypes for functions used to access physical memory addresses */
int open_physical (int);

void close_physical (int);

 /* This program increments the contents of the red LED parallel port */
int main(void)
{

//	unsigned char write_buf[4]={0b00000000,0b00000000,0b00000000,0b11111111};
	unsigned char buf[4]={0b00000000,0b00000000,0b00000000,0b11111111};
 // virtual address pointer to red LEDs
	int fd; // used to open /dev/mem // physical addresses for light-weight bridge
 // Create virtual memory access to the FPGA light-weight bridge
	if ((fd = open_physical (fd)) == -1)
		return (-1);
	printf("%d",fd);
//	printf("escreva pai");
///	scanf(" %[^\n]",buf);
 // Set virtual address pointer to I/O port
	write(fd,buf,sizeof(buf));
	close_physical (fd);
	return 0;
}


int open_physical (int fd) {
	//if (fd == -1) // check if already open
		if ((fd = open( "/dev/gpu123", (O_RDWR | O_SYNC))) == -1) {
			printf ("ERROR: could not open \"/dev/mem\"...\n");
			return (-1);
}
	return fd;
}
/* Close /dev/mem to give access to physical addresses */
void close_physical (int fd) {
	close (fd);
}