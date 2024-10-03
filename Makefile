# Compiler
CC = gcc


# Object Files
OBJS = main.o functions.o run.o stack.o

# Output Executable Name
TARGET = riscv_sim

# Default Target
all: $(TARGET)

# Linking Object Files to Create Executable
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

# Compile Source Files into Object Files
main.o: main.c
	$(CC) -c main.c

functions.o: functions.c functions.h
	$(CC) -c functions.c

run.o: run.c run.h
	$(CC) -c run.c

stack.o: stack.c stack.h
	$(CC) -c stack.c

# Clean Up Object Files and Executable
clean:
	rm -f $(OBJS) $(TARGET)
