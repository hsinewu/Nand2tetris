// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

// Put your code here.
(INIT)
	@8192
	D=A
	@maxLine
	M=D

(SET)
	// set up counter
	@maxLine
	D=M
	@counter
	M=D
	// set up ptrScreen
	@SCREEN
	D=A
	@ptrScreen
	M=D
	(LINE1)
		// iteration
		@ptrScreen
		A=M
		M=-1
		// incrementation
		@ptrScreen
		M=M+1
		@counter
		M=M-1
		D=M
		@LINE1
		D; JNE
(PRESSED)
	@KBD
	D=M
	@PRESSED
	D; JNE
	// JUMP UNSET
(UNSET)
	// set up counter
	@maxLine
	D=M
	@counter
	M=D
	// set up ptrScreen
	@SCREEN
	D=A
	@ptrScreen
	M=D
	(LINE2)
		// iteration
		@ptrScreen
		A=M
		M=0
		// incrementation
		@ptrScreen
		M=M+1
		@counter
		M=M-1
		D=M
		@LINE2
		D; JNE
(UNPRESSED)
	@KBD
	D=M
	@UNPRESSED
	D; JEQ
	@SET
	0; JEQ