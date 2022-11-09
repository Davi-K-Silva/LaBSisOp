BUILDROOT_DIR := ../../buildroot
KDIR := $(BUILDROOT_DIR)/output/build/linux-custom
COMPILER := $(BUILDROOT_DIR)/output/host/bin/i686-buildroot-linux-uclibc-gcc

all:
	gcc -o thread_runner thread_runner.c -lpthread -Wall
#   gcc -o test test.c -lpthread -Wall

target:
	$(COMPILER) -o thread_runner thread_runner.c -lpthread -Wall
	cp thread_runner $(BUILDROOT_DIR)/output/target/bin

clean:
	rm thread_runner
#	rm test