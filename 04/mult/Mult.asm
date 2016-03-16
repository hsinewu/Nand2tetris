// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

// if R0=0
//   goto EXIT
// R0--
// R2+=R1
// goto LOOP

	@R2
	M=0
(LOOP)
	@R0
	D=M
	@EXIT
	D; JEQ 
	@R0
	M=M-1
	@R1
	D=M
	@R2
	M=M+D
	@LOOP
	0; JEQ
(EXIT)
	@EXIT
	0; JEQ