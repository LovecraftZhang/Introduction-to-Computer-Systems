.pos 0x1000
code:
	# v0 = s.x[i];
	ld $v0, r0			#r0 = address of v0
	ld $i, r1			#r1 = address of i
	ld (r1), r1			#r1 = value of i
	ld $s, r2 			#r2 = address of s
	ld (r2,r1,4), r3	#r3 = s.x[i]
	st r3, (r0)			#v0 = s.x[i]

	# v1 = s.y[i];
	ld $v1, r0			#r0 = address of v1
	ld 8(r2), r3		#r3 = s.y[0]
	ld (r3, r1, 4), r4	#r4 = s.y[i]

	# v2 = s.z->x[i];
	ld $v2, r0			#r0 = address of v2
	ld 12(r2), r3		#r3 = s.z
	ld (r3, r1, 4), r4	#r4 = s.z->x[i]






.pos 0x2000
static:
	i:	.long 0
	v0: .long 0
	v1: .long 0
	v2: .long 0
	v3: .long 0

s:	.long 1
	.long 0
    .long s_y
    .long s_z
.pos 0x3000
heap:
	s_y:
		.long 2		# s.y[0]
		.long 0		# s.y[1]
	s_z:   
		.long 0		# s.z->x[0]
    	.long 0		# s.z->x[1]
    	.long 0		# s.z->y
    	.long s_z_z # s.z->z
    s_z_z:
		.long 0		# s.z.z->x[0]
    	.long 0		# s.z.z->x[1]
    	.long s_z_z_y	# s.z.z->y
    s_z_z_y:
    	.long 0		# s.z.z->y[0]
    	.long 0		# s.z.z->y[1]