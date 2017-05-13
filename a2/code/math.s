.pos 0x100
                 ld   $a, r0              # r0 = address of a
                 ld   $b, r1              # r1 = address of b
                 ld   $0x1, r2            # r2 = 1
                 ld   (r1), r3            # r3 = value of temp value is b
                 add  r2, r3              # r3 = value of b + 1
                 ld   $0x4, r4            # r4 = 4
                 add  r4, r3              # r3 = value of (b + 1) + 4
                 shr  $0x1, r3            # r3 = value of ((b + 1) + 4) >> 1
                 ld   (r1), r5            # r5 = value of b
                 and  r5, r3              # r3 = value of (((b + 1) + 4) >> 1) & b
                 shl  $0x2, r3            # r3 = value of ((((b + 1) + 4) >> 1) & b) << 2
                 st   r3, (r0)            # r0 = value of the result
                 halt                     # halt
.pos 0x1000
a:               .long 0xffffffff         # a
.pos 0x2000
b:               .long 0x00000001         # b