all: main.c functions.c run.c stack.c
		gcc -o riscv_sim main.c functions.c run.c stack.c 

clean:
		$(RM) riscv_sim
