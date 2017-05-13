.pos 0x0
                 ld   $0x1028, r5       # r5 = address after sb      
                 ld   $0xfffffff4, r0   # r0 = -12 = -(sizeof ping's frame)  
                 add  r0, r5              # allocate foo's frame
                 ld   $0x200, r0          # r0 = &i
                 ld   0x0(r0), r0         # r0 = i
                 st   r0, 0x0(r5)         # insert i in stack frame
                 ld   $0x204, r0          # r0 = &j
                 ld   0x0(r0), r0         # r0 = j
                 st   r0, 0x4(r5)         # insert j in stack frame
                 ld   $0x208, r0          # r0 = &k
                 ld   0x0(r0), r0         # r0 = k
                 st   r0, 0x8(r5)         # insert k in stack frame
                 gpc  $6, r6              # save ra
                 j    0x300               # jump to ping(i, j, k)
                 ld   $0x20c, r1          # r1 = & result
                 st   r0, 0x0(r1)         # result = ping(i,j,k)
                 halt                     
.pos 0x200
                 .long 0x00000000         # i
                 .long 0x00000000         # j
                 .long 0x00000000         # k
                 .long 0x00000000         # result
.pos 0x300
ping:            ld   0x0(r5), r0         # r0 = i     
                 ld   0x4(r5), r1         # r1 = j
                 ld   0x8(r5), r2         # r2 = k
                 ld   $0xfffffff6, r3     # r3 = -10
                 add  r3, r0              # r0 = i - 10
                 mov  r0, r3              # r3 = r0 = i - 10
                 not  r3                  # r3 = !(i - 10)
                 inc  r3                  # r3 = 10 - i
                 bgt  r3, L6              # if (i < 10) go to default
                 mov  r0, r3              # r3 = i - 10
                 ld   $0xfffffff8, r4     # r4 = -8
                 add  r4, r3              # r3 = i - 18
                 bgt  r3, L6              # if (a > 18) got default
                 ld   $0x400, r3          # r3 = &jmptable[0]
                 j    *(r3, r0, 4)        # goto juptable[i - 10]
.pos 0x330
case10:          add  r1, r2              # r2 = j + k  
                 br   L7                  # goto end_switch
case12:          not  r2                  # r2 = !k
                 inc  r2                  # r2 = -k
                 add  r1, r2              # r2 = j - k
                 br   L7                  # goto end_switch
case14:          not  r2                  # r2 = !k
                 inc  r2                  # r2 = -k
                 add  r1, r2              # r2 = j - k
                 bgt  r2, L0              # if (j > k) goto L0
                 ld   $0x0, r2            # r2 = 0
                 br   L1                  # goto L1
L0:              ld   $0x1, r2            # r2 = 1
L1:              br   L7                  # goto end_switch
case16:          not  r1                  # r1 = !j
                 inc  r1                  # r1 = -j
                 add  r2, r1              # r1 = k - j
                 bgt  r1, L2              # if (j < k) goto L2
                 ld   $0x0, r2            # r2 = 0
                 br   L3                  # goto L3
L2:              ld   $0x1, r2            # r2 = 1
L3:              br   L7                  # goto end_switch
case18:          not  r2                  # r2 = !k
                 inc  r2                  # r2 = -k
                 add  r1, r2              # r2 = j - k
                 beq  r2, L4              # if (j == k) goto L4
                 ld   $0x0, r2            # r2 = 0
                 br   L5                  # goto L5
L4:              ld   $0x1, r2            # r2 = 1
L5:              br   L7                  # goto end_switch
default:         ld   $0x0, r2           # r2 = 0           
                 br   L7                 # goto end_switch 
end_switch:      mov  r2, r0              # r0 = result
                 j    0x0(r6)             # jump
.pos 0x400
                 .long 0x00000330         # case a == 10
                 .long 0x00000384         # case default
                 .long 0x00000334         # 
                 .long 0x00000384         
                 .long 0x0000033c         
                 .long 0x00000384         
                 .long 0x00000354         
                 .long 0x00000384         
                 .long 0x0000036c         
.pos 0x1000
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
