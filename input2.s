.data
.dword 1, 2, 3, 4, 5
.word 10, 20, 30, 40
.half 100, 200, 300
.byte 1, 2, 3, 4
.text
lui x1, 0x10
lui x2, 0x10
addi x2, x2, 40
lui x3, 0x10
addi x3, x3, 56
lui x4, 0x10
addi x4, x4, 62
ld x5, 0(x1)
ld x6, 8(x1)
sd x5, 16(x1)
ld x7, 16(x1)
lw x8, 0(x2)
lw x9, 4(x2)
sw x8, 8(x2)
lw x10, 8(x2)
lh x11, 0(x3)
lh x12, 2(x3)
sh x11, 4(x3)
lh x13, 4(x3)
lb x14, 0(x4)
lb x15, 1(x4)
sb x14, 2(x4)
lb x16, 2(x4)