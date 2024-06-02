obj-m += kernelgpudriver.o

all:
	make -C /home/valmir/gpu_driver/WSL2-Linux-Kernel M=$(shell pwd) modules

	
	sudo insmod kernelgpudriver.ko; \

	if [ ! -e /dev/gpu123 ]; then \
		sudo mknod /dev/gpu123 c 240 0; \
	fi

program: instructions.o gpu.o
	gcc -o gpu gpu.o instructions.o

instructions.o: lib/instructions.c
	gcc -c lib/instructions.c -o instructions.o

gpu.o: lib/gpu.c
	gcc -c lib/gpu.c -o gpu.o

clean:
	make -C /home/valmir/gpu_driver/WSL2-Linux-Kernel M=$(shell pwd) clean
	rm -f gpu.o instructions.o
