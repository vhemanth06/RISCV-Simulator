.data
.dword -123456789,5,-121
.text
lui x3, 0x10
ld x4, 0(x3)
ld x5, 8(x3)
slli x6,x4,12
srli x7,x4,12
srai x8,x4,12