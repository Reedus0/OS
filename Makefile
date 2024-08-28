SRC_DIR := src
BUILD_DIR := build
DIST_DIR := dist

AS := nasm
CC := gcc
LD := ld
GRUB_MKRESCUE := grub-mkrescue

QEMU := qemu-system-x86_64
BOCHS := bochs

QEMU_FLAGS := -m 1024 -drive format=raw,file=dist/x86_64/kernel.iso -drive format=raw,if=ide,file=fs
BOCHS_FLAGS := -f bochs_config.bxrc

# x86_64 target

ASM_FLAGS := -f elf64
CC_FLAGS := -I $(SRC_DIR) -I $(SRC_DIR)/arch/x86_64 -ffreestanding -MMD -fno-asynchronous-unwind-tables
LD_FLAGS := -n --allow-multiple-definition -T target/x86_64/linker.ld

ISO_DIR := target/x86_64/iso

ASM_FILES := $(shell find $(SRC_DIR) -name *.asm)
C_FILES := $(shell find $(SRC_DIR) -name *.c)
DEPEND_FILES := $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.d)
OBJ_FILES := $(ASM_FILES:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.o) $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

-include $(DEPEND_FILES)

TARGET := $(DIST_DIR)/x86_64/kernel.bin
ISO := $(DIST_DIR)/x86_64/kernel.iso

$(ISO): $(TARGET)
	@mkdir -p $(ISO_DIR)/boot
	cp $(TARGET) $(ISO_DIR)/boot/kernel.bin
	$(GRUB_MKRESCUE) /usr/lib/grub/i386-pc -o $(ISO) $(ISO_DIR)

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(DIST_DIR)
	@mkdir -p $(DIST_DIR)/x86_64
	$(LD) $(LD_FLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(@D)
	$(AS) $(ASM_FLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -c $< -o $@

build-x86_64: $(ISO)

run: $(ISO)
	make build-x86_64
	$(QEMU) $(QEMU_FLAGS)

dbg: $(ISO)
	make build-x86_64

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)
