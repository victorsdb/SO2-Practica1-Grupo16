obj-m += cpu_grupo16.o
cpu_grupo16-objs += main.o
cpu_grupo16-objs += cpu.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean