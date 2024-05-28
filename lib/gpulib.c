#include "instructions.h"
#include <stdint.h>

volatile int *DATA_A_PTR, *DATA_B_PTR; // virtual addresses

void setBackground(uint8_t R, uint8_t G, uint8_t B);

void setPolygon(uint8_t relX, uint8_t relY, Polygon polygon);

void setSprite();

void setBackground(uint8_t R, uint8_t G, uint8_t B) {
    assembleInstructionWBR(DATA_A_PTR, DATA_B_PTR, 0, R, G, B);

    if ((fd = open_physical (fd)) == -1)
		return (-1);
    
    // unsigned char buf[2] = {DATA_A_PTR, DATA_B_PTR};

	write(fd,buf,sizeof(buf));
	close_physical (fd);

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
