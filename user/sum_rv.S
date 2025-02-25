.data
buffer: .asciz "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
.byte 0
no_space_err: .asciz "error: no space\n"
.byte 0
not_int_err: .asciz "error: not an integer\n"
.byte 0
format: .asciz "%d\n"
.byte 0
zero_str: .asciz "0"
.byte 0
zero_newline_str: .asciz "0\n"
.byte 0

.text
.global main

main:
	la a0, buffer
	call gets
	
	li a1, ' '
	call strchr
	
	beq a0, zero, no_space
	
	sb zero, (a0)
	addi a0, a0, 1
	
	mv s1, a0
	call atoi
	
	bne a0, zero, snd_number_read
	
	mv a0, s1
	la a1, zero_newline_str
	call strcmp
	
	bne a0, zero, not_int
	mv zero, s1
	
snd_number_read:
	mv s1, a0
	
	la a0, buffer
	call atoi
	
	bne a0, zero, fst_number_read
	
	la a0, buffer
	la a1, zero_str
	call strcmp
	
	bne a0, zero, not_int
	
fst_number_read:
	add a1, a0, s1
	la a0, format
	call printf
	
	li a7, 2
	li a0, 0
	ecall
	
no_space:
	la a0, no_space_err
	call printf
	
	li a7, 2
	li a0, 1
   	ecall
	
not_int:
	la a0, not_int_err
	call printf

	li a7, 2
	li a0, 1
   	ecall
