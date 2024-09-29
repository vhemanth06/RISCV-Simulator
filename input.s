.data
.dword 21, 14
.text
lui x3, 0x10    
ld x5, 0(x3)       
ld x6, 8(x3)       
GCD_LOOP: beq x6, x0, GCD_FOUND  
blt x5, x6, Swap_Numbers 
sub x5, x5, x6         
beq x0, x0, GCD_LOOP   
Swap_Numbers: add x7, x0, x5         
add x5, x0, x6         
add x6, x0, x7         
beq x0, x0, GCD_LOOP   
GCD_FOUND: sd x5, 16(x3)