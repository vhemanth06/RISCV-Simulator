.data
.dword -123456789,5,-121
.text
lui x3, 0x10
addi x4, x0, 5
loop: addi x4, x4, -1
beq x0,x0,loop