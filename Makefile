TARGET       := fermios

VER_MAJ      := 1
VER_MIN      := 1
VER_FIX      := 1
VER_SUF      := a
VER          := $(VER_MAJ).$(VER_MIN).$(VER_FIX)$(VER_SUF)

ARCH         := i386

BIN_DIR      := bin
OBJ_DIR      := obj
SRC_DIR      := src
INC_DIR      := include
SYSROOT_DIR  := sysroot
CROSS_DIR    := cross

AS           := nasm
CC           := i686-elf-gcc
AR           := i686-elf-ar

AFLAGS       := -f elf32
CFLAGS_DEBUG := -g -D _DEBUG
CFLAGS       := -Wall -Wextra -std=gnu11 -O2 -ffreestanding $(CFLAGS_DEBUG) \
	-D_KERNEL_VERSION=\"$(VER)\"
LFLAGS       := -nostdlib -lgcc -T $(SRC_DIR)/arch/$(ARCH)/linker.ld -L$(BIN_DIR)
INCFLAGS     := -I$(abspath $(INC_DIR)) -I$(abspath $(INC_DIR))/libc
QFLAGS       := -enable-kvm -monitor stdio -m 1024M -smp 2 -display gtk

KRN_ASM      := $(shell find $(SRC_DIR)/arch/$(ARCH) -type f -name "*.s")
KRN_SRC      := $(shell find $(SRC_DIR)/kernel -type f -name "*.c") \
	$(SRC_DIR)/arch/$(ARCH)/tty.c \
	$(SRC_DIR)/arch/$(ARCH)/gdt.c
KRN_OBJ      := $(addprefix $(OBJ_DIR)/, $(KRN_ASM:%.s=%.o)) \
	$(addprefix $(OBJ_DIR)/, $(KRN_SRC:%.c=%.o))             \
	$(OBJ_DIR)/crtbegin.o                                    \
	$(OBJ_DIR)/crtend.o

LIBK_SRC     := $(shell find $(SRC_DIR)/libc -type f -name "*.c")
LIBK_OBJ     := $(addprefix $(OBJ_DIR)/, $(LIBK_SRC:%.c=%.libk.o))

define ISO_GRUB_CFG
set timeout=15
set default=0

menuentry "$(TARGET)-$(VER)-$(ARCH)" {
	multiboot /boot/$(TARGET).bin
	boot
}
endef

export ISO_GRUB_CFG
export PATH  := $(CROSS_DIR)/bin:$(PATH)


.PHONY: all rbr rebuild brun build run run-iso clean

all: rebuild

rbri : rebuild run-iso

rbr: rebuild run

rebuild: clean build

bruni: build run-iso

brun: build run

build: $(TARGET)-$(VER)-$(ARCH).iso

run:
	qemu-system-i386 $(QFLAGS) -kernel $(BIN_DIR)/$(TARGET).bin

run-iso:
	qemu-system-i386 $(QFLAGS) -cdrom $(TARGET)-$(VER)-$(ARCH).iso

$(TARGET)-$(VER)-$(ARCH).iso: $(BIN_DIR)/$(TARGET).bin
	mkdir -p $(SYSROOT_DIR)/boot/grub
	cp $(BIN_DIR)/$(TARGET).bin $(SYSROOT_DIR)/boot/.

	echo "$$ISO_GRUB_CFG" > $(SYSROOT_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(SYSROOT_DIR)

$(BIN_DIR)/$(TARGET).bin: $(BIN_DIR)/libk.a $(KRN_OBJ)
	$(CC) $(LFLAGS) -o $@ $(CFLAGS) $(KRN_OBJ) -lk $(INCFLAGS)
	grub-file --is-x86-multiboot $@

$(OBJ_DIR)/crtbegin.o $(OBJ_DIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS) $(INCFLAGS) -D__is_kernel

$(OBJ_DIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(AS) $(AFLAGS) $< -o $@

$(BIN_DIR)/libk.a: $(LIBK_OBJ)
	$(AR) rcs $@ $(LIBK_OBJ)

$(OBJ_DIR)/%.libk.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS) $(INCFLAGS) -D__is_libk

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(OBJ_DIR)/*
	rm -rf $(SYSROOT_DIR)
	rm -f  $(TARGET)-*.iso
