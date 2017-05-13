.pos 0x100

    #Statement 1: i = a[3]
    ld $i, r0               # r0 = address of i
    ld $a, r1               # r1 = address of a
    ld 12(r1), r2           # r2 = a[3]
    st r2, (r0)             # i = a[3]

    #Statement 2: i = a[i]
    ld (r0), r3             # r3 = i
    ld (r1, r3, 4), r3      # r3 = a[i]
    st r3, (r0)             # i = a[i]

    #Statement 3: p = &j
    ld $j, r4               # r4 = address of j
    ld $p, r5               # r5 = address of p
    st r4, (r5)             # p = &j

    #Statement 4: *p = 4 
    ld (r5), r6             # r6 = p value
    ld $4, r0               # r0 = 4
    st r0, (r6)             # *p = 4

    #Statement 5: p  = &a[a[2]]
    ld 8(r1), r2            # r2 = a[2]
    shl $2, r2              # r2 = 4 * r2
    add r1, r2              # r2 = &a + r2
    st r2, (r5)             # p  = &a[a[2]]

    #Statement 6: *p = *p + a[4]
    ld 16(r1), r2           # r2 = a[4]
    ld (r5), r6             # r6 = value of p
    ld (r6), r7             # r7 = the value of p points to
    add r2, r7              # r7 = a[4] + *p
    st r7, (r6)             # *p = a[4] + *p


    halt

.pos 0x200
# Data area

i:  .long 0             # i
j:  .long 0             # j
p:  .long 0             # p
a:  .long 0             # a[0]
    .long 0             # a[1]
    .long 0             # a[2]
    .long 4             # a[3]
    .long 5             # a[4]
    .long 0             # a[5]
    .long 0             # a[6]
    .long 0             # a[7]
    .long 0             # a[8]
    .long 0             # a[9]
