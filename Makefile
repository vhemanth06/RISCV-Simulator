all: main.c functions.c run.c
		gcc -o riscv_sim main.c functions.c run.c

clean:
		$(RM) riscv_sim
