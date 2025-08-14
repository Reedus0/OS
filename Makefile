SRC_DIR := src
BUILD_DIR := build
DIST_DIR := dist

AS := nasm
CC := gcc
LD := ld

QEMU := qemu-system-x86_64
BOCHS := bochs
GDB := gdb

QEMU_FLAGS := -m 1024 -d cpu_reset -no-reboot -drive id=disk,file=drive,if=none -device ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0
QEMU_DEBUG_FLAGS := $(QEMU_FLAGS) -s -S
GDB_FLAGS := -ex "target remote localhost:1234" -ex "symbol-file ./dist/x86_64/kernel.bin" -ex "add-symbol-file ./dist/x86_64/kernel.bin 0xffff800001000000" -ex "source ./.vscode/breakpoints.gdb" -ex "c"

# x86_64 target

ASM_FLAGS := -f elf64
CC_FLAGS := -I $(SRC_DIR) -I $(SRC_DIR)/arch/x86_64 -ffreestanding -MMD -fno-asynchronous-unwind-tables -g
LD_FLAGS := -n --allow-multiple-definition -T target/x86_64/linker.ld

ASM_FILES := $(shell find $(SRC_DIR) -name *.asm)
C_FILES := $(shell find $(SRC_DIR) -name *.c)
DEPEND_FILES := $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.d)
OBJ_FILES := $(ASM_FILES:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.o) $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

-include $(DEPEND_FILES)

TARGET := $(DIST_DIR)/x86_64/kernel.bin

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

build-x86_64: $(TARGET)
	./fs.sh

run: $(TARGET)
	make build-x86_64
	$(QEMU) $(QEMU_FLAGS)

dbg_win: $(TARGET)
	make build-x86_64
	$(BOCHS) -f bochs_config_win.bxrc

dbg_linux: $(TARGET)
	make build-x86_64
	$(BOCHS) -f bochs_config_linux.bxrc

dbg_gdb: $(TARGET)
	make build-x86_64
	$(QEMU) $(QEMU_DEBUG_FLAGS) &
	$(GDB) $(GDB_FLAGS)

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)
