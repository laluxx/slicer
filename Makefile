# Compiler and Linker
CC = gcc

# The Target Binary Program
TARGET = slicer

# Compiler and Linker flags
CFLAGS = -Wall -I.
LFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11

# Source files
SOURCES = main.c slicer.c character.c panels.c log.c screen.c ui.c modes.c window.c filemanager.c

# Compilation and linking
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LFLAGS) -o $(TARGET)
	@echo "Linking complete!"

.PHONEY: clean
clean:
	@rm -f $(TARGET)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@rm -f $(TARGET)
	@echo "Executable removed!"
