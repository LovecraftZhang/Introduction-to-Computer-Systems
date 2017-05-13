.pos 0x100

    #Statement 1: tmp = 0
    ld $tmp, r0               # r0 = address of tmp
    ld $0, r1                 # r1 = 0
    st r1, (r0)               # tmp = 0

    #Statement 2: tos = 0
    ld $tos, r2               # r2 = address of tos
    st r1, (r2)               # tos = 0

    #Statement 3: s[tos] = a[0]
    ld $a, r3                 # r3 = address of a
    ld 0(r3), r4              # r4 = a[0]
    ld $s, r5                 # r5 = address of s
    ld (r2), r6                 # r6 = tos
    st r4, (r5, r6, 4)        # s[tos] = a[0]

    #Statement 4: tos++
    inc r6

    #Statement 5: s[tos] = a[1]
    ld 4(r3), r4              # r4 = a[1]
    st r4, (r5, r6, 4)        # s[tos] = a[1]

    #Statement 6: tos++
    inc r6

    #Statement 7: s[tos] = a[2]
    ld 8(r3), r4              # r4 = a[2]
    st r4, (r5, r6, 4)        # s[tos] = a[2]

    #Statement 8: tos++
    inc r6

    #Statement 9: tos--
    dec r6

    #Statement 10: tmp = s[tos]
    ld (r5, r6, 4), r4       # r4 = s[tos]
    st r4, (r0)              # tmp = s[tos]

    #Statement 11: tos --
    dec r6

    #Statement 12: tmp = tmp + s[tos]
    ld (r0), r7                # r7 = tmp
    ld (r5, r6, 4), r4       # r4 = s[tos]
    add r4, r7               # r7 = tmp + s[tos]
    st r7, (r0)                # tmp = tmp + s[tos]

    #Statement 13: tos --
    dec r6

    #Statement 14: tmp = tmp + s[tos]
    ld (r0), r7                # r7 = tmp
    ld (r5, r6, 4), r4       # r4 = s[tos]
    add r4, r7               # r7 = tmp + s[tos]
    st r7, (r0)                # tmp = tmp + s[tos]

    halt

.pos 0x200
# Data area

tmp:  .long 0             # tmp
tos:  .long 0             # tos
a:    .long 3             # a[0]
      .long 4             # a[1]
      .long 5             # a[2]
s:    .long 0             # s[0]
      .long 0             # s[1]
      .long 0             # s[2]
      .long 0             # s[3]
      .long 0             # s[4]