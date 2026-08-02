# Makefile — cross-compile the myled-dt kernel module against the 7.1.5 kernel
# beagley-linux — Embedded Linux From Scratch
#
# Usage: make        (build myled-dt.ko)
#        make clean  (remove build artifacts)
#
# NOTE: indentation below MUST be a real TAB, not spaces.

obj-m += myled-dt.o

KDIR := $(HOME)/beagley/kernel/linux-7.1.5
ARCH := arm64
CROSS_COMPILE := aarch64-linux-gnu-

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
