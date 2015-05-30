/* Copyright (C) 2012-2015 Free Software Foundation, Inc.

   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* Assumptions:
 *
 * ARMv8-a, AArch64
 * Unaligned accesses
 *
 */

#define dstin	x0
#define src	x1
#define count	x2
#define tmp1	x3
#define tmp1w	w3
#define tmp2	x4
#define tmp2w	w4
#define tmp3	x5
#define tmp3w	w5
#define dst	x6

#define A_l	x7
#define A_h	x8
#define B_l	x9
#define B_h	x10
#define C_l	x11
#define C_h	x12
#define D_l	x13
#define D_h	x14

#include <sysdep.h>

ENTRY_ALIGN (__memcpy_thunderx, 6)
#ifdef __ILP32__
	/* Zero extend the pointers that is passed in w0 and w1. */
	/* Zero extend the size_t that was passed in w2 */
	mov	w0, w0
	mov	w1, w1
	mov	w2, w2
#endif
	prfm	pldl1strm,[src]
	mov	dst, dstin
	cmp	count, #16
	/*When memory length is less than 16, the accessed are not aligned.*/
	b.lo	L(tiny15)

	neg	tmp2, src
	ands	tmp2, tmp2, #15/* Bytes to reach alignment. */
	b.eq	L(SrcAligned)
	sub	count, count, tmp2
	/*
	* Copy the leading memory data from src to dst in an increasing
	* address order.By this way,the risk of overwritting the source
	* memory data is eliminated when the distance between src and
	* dst is less than 16. The memory accesses here are alignment.
	*/
	tbz	tmp2, #0, 1f
	ldrb	tmp1w, [src], #1
	strb	tmp1w, [dst], #1
1:
	tbz	tmp2, #1, 2f
	ldrh	tmp1w, [src], #2
	strh	tmp1w, [dst], #2
2:
	tbz	tmp2, #2, 3f
	ldr	tmp1w, [src], #4
	str	tmp1w, [dst], #4
3:
	tbz	tmp2, #3, L(SrcAligned)
	ldr	tmp1, [src],#8
	str	tmp1, [dst],#8

L(SrcAligned):
	cmp	count, #64
	b.ge	L(cpy_over64)
	/*
	* Deal with small copies quickly by dropping straight into the
	* exit block.
	*/
L(tail63):
	/*
	* Copy up to 48 bytes of data. At this point we only need the
	* bottom 6 bits of count to be accurate.
	*/
	ands	tmp1, count, #0x30
	b.eq	L(tiny15)
	cmp	tmp1w, #0x20
	b.eq	1f
	b.lt	2f
	ldp	A_l, A_h, [src], #16
	stp	A_l, A_h, [dst], #16
1:
	ldp	A_l, A_h, [src], #16
	stp	A_l, A_h, [dst], #16
2:
	ldp	A_l, A_h, [src], #16
	stp	A_l, A_h, [dst], #16
L(tiny15):
	/*
	* Prefer to break one ldp/stp into several load/store to access
	* memory in an increasing address order,rather than to load/store 16
	* bytes from (src-16) to (dst-16) and to backward the src to aligned
	* address,which way is used in original cortex memcpy. If keeping
	* the original memcpy process here, memmove need to satisfy the
	* precondition that src address is at least 16 bytes bigger than dst
	* address,otherwise some source data will be overwritten when memove
	* call memcpy directly. To make memmove simpler and decouple the
	* memcpy's dependency on memmove, withdrew the original process.
	*/
	tbz	count, #3, 1f
	ldr	tmp1, [src], #8
	str	tmp1, [dst], #8
1:
	tbz	count, #2, 2f
	ldr	tmp1w, [src], #4
	str	tmp1w, [dst], #4
2:
	tbz	count, #1, 3f
	ldrh	tmp1w, [src], #2
	strh	tmp1w, [dst], #2
3:
	tbz	count, #0, L(exitfunc)
	ldrb	tmp1w, [src]
	strb	tmp1w, [dst]

L(exitfunc):
	RET

L(cpy_over64):
	subs	count, count, #128
	b.ge	L(cpy_body_large)
	/*
	* Less than 128 bytes to copy, so handle 64 here and then jump
	* to the tail.
	*/
	ldp	A_l, A_h, [src],#16
	stp	A_l, A_h, [dst],#16
	ldp	B_l, B_h, [src],#16
	ldp	C_l, C_h, [src],#16
	stp	B_l, B_h, [dst],#16
	stp	C_l, C_h, [dst],#16
	ldp	D_l, D_h, [src],#16
	stp	D_l, D_h, [dst],#16

	tst	count, #0x3f
	b.ne	L(tail63)
	ret

	/*
	* Critical loop.  Start at a new cache line boundary.  Assuming
	*/
	.p2align	6
L(cpy_body_large):
	mov 	D_l, src
	lsr	D_h, count, #7
	cmp 	D_h, #0
	b.lt	4f
	mov	D_h, #4
4:
	prfm	pldl1strm,[D_l, 128]
	add	D_l, D_l, #128
	sub	D_h, D_h, #1
	cbnz	D_h, 4b
	/* pre-get 64 bytes data. */
	ldp	A_l, A_h, [src],#16
	ldp	B_l, B_h, [src],#16
	ldp	C_l, C_h, [src],#16
	ldp	D_l, D_h, [src],#16
1:
	/*
	* interlace the load of next 64 bytes data block with store of the last
	* loaded 64 bytes data.
	*/
	tbz	src, #6, 2f
	prfm 	pldl1strm,[src,512]
2:

	stp	A_l, A_h, [dst],#16
	ldp	A_l, A_h, [src],#16
	stp	B_l, B_h, [dst],#16
	ldp	B_l, B_h, [src],#16
	stp	C_l, C_h, [dst],#16
	ldp	C_l, C_h, [src],#16
	stp	D_l, D_h, [dst],#16
	ldp	D_l, D_h, [src],#16
	subs	count, count, #64
	b.ge	1b
	stp	A_l, A_h, [dst],#16
	stp	B_l, B_h, [dst],#16
	stp	C_l, C_h, [dst],#16
	stp	D_l, D_h, [dst],#16

	tst	count, #0x3f
	b.ne	L(tail63)
	RET
END (__memcpy_thunderx)
libc_hidden_builtin_def (__memcpy_thunderx)
