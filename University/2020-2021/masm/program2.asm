TITLE Program Template     (program2.asm)

; Author:					Aaron Frost
; Last Modified:			1/21/2021
; OSU email address:		frostaa@oregonstate.edu
; Course number/section:	CS271 001 W2021
; Assignment Number:		2							Due Date:	1/24/2021
; Description:			Recieves standard input for number of fibonacci terms to display (n), then uses an iterative loop to output n fibonacci terms starting at 1

INCLUDE Irvine32.inc

.data

	MAX_FIB_AMOUNT = 46
	MIN_FIB_AMOUNT = 1
	TERMS_PER_LINE = 4

	current_row_length		DWORD	0

	user_name				BYTE	32 DUP(0)	

	fib_amount				DWORD	?

	prev1					DWORD	1
	prev2					DWORD	0


	string_start			BYTE	"~~~~ Fibonacci Numbers by Aaron Frost ~~~~", 0

	string_desc				BYTE	"This program will output up to 46 fibonacci numbers.", 0

	string_prompt1			BYTE	"Enter Your Name: ", 0

	string_greeting			BYTE	"Hello, ", 0

	string_prompt2			BYTE	"Please enter how many Fibonacci Numbers you would like to display (Range[1, 46]): ", 0

	string_invalid_amount	BYTE	"The amount you entered is not in the range previously defined.", 0

	string_space			BYTE	"     ", 0

	string_end				BYTE	"This program will now terminate. Goodbye " , 0

	string_punc_period		BYTE	".", 0



.code
main PROC

	; Section Introduction:
	; outputs my name and the program title to the console
	; then gets the user's name, and greets them with a console message

	introduction:
		mov		edx	, OFFSET string_start
		call	WriteString
		call	Crlf

		mov		edx	, OFFSET string_desc
		call	WriteString
		call	Crlf

		mov		edx	, OFFSET string_prompt1
		call	WriteString

		mov		edx , OFFSET user_name
		mov		ecx , SIZEOF user_name

		call	ReadString

		call	Crlf

		mov		edx, OFFSET string_greeting
		call	WriteString

		mov		edx, OFFSET user_name
		call	WriteString

		mov		edx, OFFSET string_punc_period
		call	WriteString

		call	Crlf



	




	; Section Display Instructions:
	; Prompts the user for the amount of fibonacci numbers to display
	display_instructions:
		mov		edx, OFFSET string_prompt2
		call	WriteString
		call	Crlf







	; Section Get User Input
	; Reads and validates the amount of fibonacci numbers to display
	get_fib_amount:
		call	ReadInt
		mov		fib_amount, eax
		cmp		fib_amount, MIN_FIB_AMOUNT
		jl		invalid_input					; jump to invalid if input is too low
		cmp		fib_amount, MAX_FIB_AMOUNT
		jg		invalid_input					; jump to invalid if input is too high
		jmp		valid_input


	; This code block executes if the user input is out of range
	; Write an error message, then prompt for input again
	invalid_input:
		mov		edx, OFFSET string_invalid_amount
		call	WriteString
		call	Crlf
		jmp		display_instructions

	
	; This code block executes if the user input is within range
	; Initialize the eax and ecx registers for the fibonacci calculations
	valid_input:
		mov		ecx, fib_amount
		mov		eax, 1


	






	; Section Display Fibonacci Sequence
	; This is a looping code label that calculates each fibonacci number and outputs to console
	display_fib:
		call	WriteDec

		; EAX = prev1 + prev2
		mov		eax, prev1
		add		eax, prev2

		; prev2 = prev1
		mov		ebx, prev1
		mov		prev2, ebx

		; prev1 = EAX
		mov		prev1, eax

		mov		edx, OFFSET string_space
		call	WriteString

		inc		current_row_length

		cmp		current_row_length, TERMS_PER_LINE
		je		start_new_row								; if current row length is at its max, start a new row

		loop	display_fib

		call	Crlf
		jmp		goodbye


	; This code block gets executed if the current_row_length has reached the TERMS_PER_ROW constant
	; This just jumps the console to a new line and continues the loop
	start_new_row:
		mov		current_row_length, 0
		call	Crlf
		loop	display_fib









	; Section Goodbye
	; Outputs a goodbye message to the user in the console
	goodbye:
		mov		edx, OFFSET string_end
		call	WriteString
		mov		edx, OFFSET user_name
		call	WriteString
		mov		edx, OFFSET string_punc_period
		call	WriteString
		call	Crlf


	exit	; exit to operating system
main ENDP


; (insert additional procedures here)

END main
