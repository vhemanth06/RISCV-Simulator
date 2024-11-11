.data
.dword 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8
.text
lui x3,0x10
ld x4,0(x3)
ld x5,16(x3)
ld x6,32(x3)
ld x7,48(x3)
ld x8,64(x3)
ld x10,16(x3)
ld x9,80(x3)
ld x12,0(x3)
ld x12,32(x3)
