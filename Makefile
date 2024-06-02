obj-m += kernelteste.o

all:
	make -C /home/valmir/gpu_driver/WSL2-Linux-Kernel M=$(shell pwd) modules

program:
	gcc -c lib/instructions.c
	gcc -c lib/gpu.c
	gcc -o gpu gpu.o instructions.o
clean:
	make -C /home/valmir/gpu_driver/WSL2-Linux-Kernel M=$(shell pwd) clean
	rm -f gpu.o instructions.o