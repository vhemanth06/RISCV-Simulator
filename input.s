.data
.dword 25,-16,-121
.text
lui x3, 0x10
lwu x5, 9(x3)
lhu x6, 9(x3)
lbu x7, 9(x3)
add x9,x5,x7
