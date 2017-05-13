.pos 0x0
                 ld   $0x1028, r5               # r5 = address 1028
                 ld   $0xfffffff4, r0           # r0 = -12
                 add  r0, r5                    # allocate ping's frame, r5 = 1028 - 12 = 1016
                 ld   $0x200, r0                # r0 = &i
                 ld   0x0(r0), r0               # r0 = i
                 st   r0, 0x0(r5)               # local variable 1 = i
                 ld   $0x204, r0                # r0 = &j
                 ld   0x0(r0), r0               # r0 = j
                 st   r0, 0x4(r5)               # local variable 2 = j
                 ld   $0x208, r0                # r0 = &k
                 ld   0x0(r0), r0               # r0 = k
                 st   r0, 0x8(r5)               # local variable 3 = k
                 gpc  $6, r6                    # save ra to r6
                 j    0x300                     # call ping
                 ld   $0x20c, r1                # r1 = &result
                 st   r0, 0x0(r1)               # result = ping(i, j, k)
                 halt                     
.pos 0x200
i:               .long 0x00000000               # i     
j:               .long 0x00000000               # j
k:               .long 0x00000000               # k
result:          .long 0x00000000               # result
.pos 0x300
                 ld   0x0(r5), r0               # r0 = i
                 ld   0x4(r5), r1               # r1 = j
                 ld   0x8(r5), r2               # r2 = k
                 ld   $0xfffffff6, r3           # r3 = -10
                 add  r3, r0                    # r0 = i - 10
                 mov  r0, r3                    # r3 = i - 10
                 not  r3                        # r3 = ~(i - 10)
                 inc  r3                        # r3 = 10 - i
                 bgt  r3, default               # goto default if (i < 10)
                 mov  r0, r3                    # r3 = i - 10
                 ld   $0xfffffff8, r4           # r4 = -8
                 add  r4, r3                    # r4 = i - 18
                 bgt  r3, default               # goto default if (i > 18)
                 ld   $0x400, r3                # r3 = &jumptable[0]
                 j    *(r3, r0, 4)              # goto jumptable[i-10]
.pos 0x330
case10:          add  r1, r2                    # r2 = j + k     
                 br   switch_end                # goto switch_end
case12:          not  r2                        # r2 = ~k
                 inc  r2                        # r2 = -k
                 add  r1, r2                    # r2 = j - k
                 br   switch_end                # goto switch_end
case14:          not  r2                        # r2 = ~k
                 inc  r2                        # r2 = -k
                 add  r1, r2                    # r2 = j - k
                 bgt  r2, L0                    # goto L0 if (j > k)
                 ld   $0x0, r2                  # r2 = 0
                 br   L1                        # goto L1
L0:              ld   $0x1, r2                  # r2 = 1
L1:              br   switch_end                # goto switch_end        
case16:          not  r1                        # r1 = ~j
                 inc  r1                        # r1 = -j
                 add  r2, r1                    # r1 = k - j
                 bgt  r1, L2                    # goto L2 if (k > j)
                 ld   $0x0, r2                  # r2 = 0
                 br   L3                        # goto L3
L2:              ld   $0x1, r2                  # r2 = 1
L3:              br   switch_end                # goto switch_end
case18:          not  r2                        # r2 = ~k
                 inc  r2                        # r2 = -k
                 add  r1, r2                    # r2 = j - k
                 beq  r2, L4                    # goto L4 if (j == k)
                 ld   $0x0, r2                  # r2 = 0
                 br   L5                        # goto L5
L4:              ld   $0x1, r2                  # r2 = 1
L5:              br   switch_end                # goto switch_end
default:         ld   $0x0, r2                  # r2 = 0
                 br   switch_end                # goto switch_end 
switch_end:      mov  r2, r0                    # r0 = r2 = return value
                 j    0x0(r6)                   # goto main
.pos 0x400
jump_table:      .long 0x00000330         # case i == 10
                 .long 0x00000384         # case i == 11, goto default
                 .long 0x00000334         # case i == 12
                 .long 0x00000384         # case i == 13, goto default
                 .long 0x0000033c         # case i == 14
                 .long 0x00000384         # case i == 15, goto default    
                 .long 0x00000354         # case i == 16
                 .long 0x00000384         # case i == 17, goto default
                 .long 0x0000036c         # case i == 18
.pos 0x1000
                 .long 0x00000000         # 1000
                 .long 0x00000000         # 1004
                 .long 0x00000000         # 1008
                 .long 0x00000000         # 1012
                 .long 0x00000000         # 1016
                 .long 0x00000000         # 1020
                 .long 0x00000000         # 1024
                 .long 0x00000000         # 1028
                 .long 0x00000000         # 1032
stack:           .long 0x00000000         # 1036
