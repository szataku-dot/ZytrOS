ARCH ?= x86_32

OBJ_DIR := obj-$(ARCH)
BIN_DIR := bin-$(ARCH)

# Zmienne plików
OBJ := $(OBJ_DIR)/stub.o
BIN := $(BIN_DIR)/kernel_32bit

all: $(BIN)

# Tworzenie pliku wynikowego
$(BIN): $(OBJ)
	@mkdir -p $(BIN_DIR)
	ld -m elf_i386 -Ttext 0x100000 -o $@ $^

# Tworzenie pliku obiektowego
$(OBJ): src/stub.asm
	@mkdir -p $(OBJ_DIR)
	nasm -f elf32 src/stub.asm -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: distclean
distclean:
	rm -rf obj-* bin-*