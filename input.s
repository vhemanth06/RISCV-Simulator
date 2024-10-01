.data
.dword 21, 14
.text
lui x3,0x10
ld x4,0(x3)
ld x5,8(x3)
add x6,x4,x5
sd x6,0(x3)
ld x4,0(x3)
