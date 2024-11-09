CC = gcc

OBJS = main.o functions.o run.o stack.o

TARGET = riscv_sim

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -lm

main.o: main.c
	$(CC) -c main.c

functions.o: functions.c functions.h
	$(CC) -c functions.c

run.o: run.c run.h
	$(CC) -c run.c

stack.o: stack.c stack.h
	$(CC) -c stack.c

clean:
	rm -f $(OBJS) $(TARGET)
