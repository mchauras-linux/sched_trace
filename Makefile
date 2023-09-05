obj-m += sched_trace.o

EXTRA_CFLAGS = -I$(src)

VMLINUX_H = vmlinux.h
VMLINUX_TXT = vmlinux.txt

KERNEL_SRC ?= /usr/lib/modules/$(shell uname -r)/build

VMLINUX ?= $(KERNEL_SRC)/vmlinux

all: $(VMLINUX_H)
	make -C $(KERNEL_SRC) M=$(PWD) modules

clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean
	rm -f $(VMLINUX_H)

$(VMLINUX_H): $(VMLINUX_TXT) $(VMLINUX)
	pahole -C file://vmlinux.txt $(VMLINUX) > $@
