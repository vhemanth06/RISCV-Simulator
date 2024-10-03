# Makefile for the project

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Source files
SRC = main.c functions.c run.c stack.c 

# Object files
OBJ = $(SRC:.c=.o)

# Executable name
EXEC = riscv_sim

# Default target
all: $(EXEC)

# Rule to link object files into an executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Rule to compile .c files into .o files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(EXEC)

# Phony targets
.PHONY: all clean
