;Tucker Evans
;CS3871
;A collection of Assembly programs written from c code


;1
; char i,j;
;
; i=3;
; j = i+i;

start:
	MOV	A, #0
	MOV	R0, #3
	MOV	A, R0
	ADD	A, R0
	MOV	R1, A
	JMP start


;2
; char a,b;
;
; a=64;
; while (a > 0) {
;         b = a;
;         a--;
; }

start:
	MOV A, #64
test:
	JNZ loop
	JMP start


loop:
	MOV R0, A
	DEC A
	JMP test

;3
; char i,j,k;
; i=3;
; j=5;
; if (i == j)
;         k=8;
; else
;         k=9;

start:

	MOV R0, #3
	MOV	R1,	#5

	MOV A, R0
	MOV #40h, A
	CJNE R1, #40h, false
	MOV R3, #8
	JMP start

false:
	MOV R3, #9
	JMP start

;4
; char buf[64];
; char i;
;
; for (i=0; i<64; i++)
;         buf[i]=i;

start:
	MOV R0, #0
	MOV R1, #20h
	MOV A, R0
l1:
	CJNE A, #64, l2
	JMP start
l2:
	MOV @R1, A
	INC A
	INC R1
	JMP l1
