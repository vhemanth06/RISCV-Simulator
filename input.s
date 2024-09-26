.data
.dword 25,-16,-121
.text
lui x3, 0x10
ld x5, 0(x3)
ld x6, 8(x3)
ld x7, 16(x3)
add x9,x5,x7
