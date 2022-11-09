obj-m := thread_test.o
BUILDROOT_DIR := ../buildroot
KDIR := $(BUILDROOT_DIR)/output/build/linux-custom
COMPILER := $(BUILDROOT_DIR)/output/host/bin/i686-buildroot-linux-uclibc-gcc

all:
	$(COMPILER) -o thread_test thread_test.c
	cp thread_test $(BUILDROOT_DIR)/output/target/bin
	
clean:
	rm -f *.o *.ko .*.cmd
	rm -f thread_test.mod.c
	rm -f thread_test
