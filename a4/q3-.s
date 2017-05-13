.pos 0x1000
code:


# Statement 1: v0 = s.x[i]
ld $v0, r0						# r0 = $v0
ld $i, r1 						# r1 = $i
ld (r1), r1						# r1 = i
ld $s, r2						# r2 = $s
ld (r2, r1, 4), r2				# r2 = s.x[i]
st r2, (r0)						# v0 = s.x[i]

# Statement 2: v1 = s.y[i]
ld $v1, r4						# r4 = $v1
ld $s, r3						# r3 = $s
ld 8(r3), r3					# r3 = s.y
ld (r3, r1, 4), r3				# r3 = s.y[i]
st r3, (r4)						# v1 = s.y[i]

# Statement 3: v2 = s.z->x[i]
ld $v2, r4						# r4 = $v2
ld $s, r3						# r3 = $s
ld 12(r3), r3					# r3 = s.z
ld (r3, r1, 4), r3				# r3 = s.z->x[i]
st r3, (r4)						# v2 = s.z->x[i]

# Statement 4: v3 = s.z->z->y[i]
ld $v3, r4						# r4 = $v3
ld $s, r3						# r3 = $s
ld 12(r3), r3					# r3 = s.z
ld 12(r3), r3					# r3 = s.z->z
ld 8(r3), r3                    # r3 = s.z->z->y
ld (r3, r1, 4), r3				# r3 = s.z->z->y[i]
st r3, (r4)						# v3 = s.z->z->y[i]

halt

.pos 0x2000
static:
i:			.long 0
v0:			.long 0
v1:			.long 0
v2:			.long 0
v3:			.long 0
s:			.long 5				# s.x[0]
			.long 0				# s.x[1]
			.long s_y			# s.y
			.long s_z			# s.z

.pos 0x3000
heap:
s_y:		.long 4				# s.y[0]
			.long 0				# s.y[1]
s_z:		.long 6				# s.z->x[0]
			.long 0				# s.z->x[1]
			.long 0				# s.z->y
			.long s_z_z			# s.z->z
s_z_z:      .long 0				# s.z->z->x[0]
			.long 0 			# s.z->z->x[1]
			.long s_z_z_y 		# s.z->z->y
s_z_z_y:    .long 7 			# s.z->z->y[0]
			.long 0 			# s.z->z->y[1]
