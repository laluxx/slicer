# Compiler and Linker
CC = gcc

# The Target Binary Program
TARGET = slicer

# Compiler and Linker flags
CFLAGS = -Wall
LFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11

# Compilation and linking
all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c $(LFLAGS) -o $(TARGET)
	@echo "Linking complete!"

.PHONEY: clean
clean:
	@rm -f $(TARGET)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@rm -f $(TARGET)
	@echo "Executable removed!"
