.data
.dword 16,16
.text
lui x3, 0x10
ld x4, 0(x3)
ld x5, 8(x3)
addi x3, x3, 16
addi x10, x0, 0
add x12, x3, x0
If1:beq x4, x10, end1
addi x11, x0, 0
slli x15, x10, 3
add x12, x3, x15
If2:beq x5, x11, end2
sw x20, 0(x12)
slli x15, x5, 3
add x12, x12, x15
addi x11, x11, 1
beq x0, x0, If2
end2:addi x10, x10, 1
beq x0, x0, If1
end1: add x0, x0, x0