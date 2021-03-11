TITLE Program Template     (program3.asm)

; Author:					Aaron Frost
; Last Modified:			1/26/2021
; OSU email address:		frostaa@oregonstate.edu
; Course number/section:	CS271 001 W2021
; Assignment Number:		3							Due Date:	1/24/2021
; Description:			Recieves standard input for numbers in a given range until user enters a non negative, accumulating the average and the sum within variables



; NOTE: 
;			The assignment asks that we display the number of negative numbers entered, and display a special message if there were none.

;			I can only assume this means that we display the number of VALID negative numbers entered, as the Example Program Operation output
;				only displays a 4 on the assignment page, when in fact 5 negative numbers were entered. 

;			The semantics are a little weird so my "special message" just outputs
;					if there are no VALID negative numbers inputted. I think that is what the question is really asking.

INCLUDE Irvine32.inc

.data

	LOW_LIMIT = -100

	user_name				BYTE	32 DUP(0)	

	av						DWORD	0
	sum						DWORD	0
	n						DWORD	0
	cur						DWORD	0


	string_start			BYTE	"~~~~ Integer Accumulator by Aaron Frost ~~~~", 0

	string_desc				BYTE	"This program allows you to input a series of numbers from which the average and sum are calculated", 0

	string_prompt1			BYTE	"Enter Your Name: ", 0

	string_greeting			BYTE	"Hello, ", 0

	string_prompt2_1		BYTE	"Please enter numbers within (Range[-1, ", 0
	string_prompt2_2		BYTE	"]), or enter any non-negative number to generate results: ", 0

	string_invalid_amount	BYTE	"The amount you entered is not in the range previously defined.", 0

	string_prompt3			BYTE	"Enter a number: ", 0


	string_n_1				BYTE	"You entered ", 0
	string_n_2				BYTE	" valid negative numbers.", 0


	string_no_valid_numbers	BYTE	"You didn't enter any valid negative numbers!", 0

	string_sum				BYTE	"The sum of your valid numbers is ", 0
	string_av				BYTE	"The rounded average of your valid numbers is ", 0

	string_space			BYTE	"     ", 0

	string_end				BYTE	"This program will now terminate. Goodbye " , 0

	string_punc_period		BYTE	".", 0





.code
main PROC

	; Section Introduction:
	; 1. outputs my name and the program title to the console
	; 2. gets the user's name, and greets them with a console message

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



	; Outputs the rules for user to follow
	display_instructions:
		mov		edx, OFFSET string_prompt2_1
		call	WriteString

		mov		eax, LOW_LIMIT
		call	WriteInt

		mov		edx, OFFSET string_prompt2_2
		call	WriteString
		call	Crlf



	; Section Get User Input, validate range
	accumulate:
		mov		edx, OFFSET string_prompt3
		call	WriteString

		call	ReadInt
		mov		cur, eax
		cmp		cur, LOW_LIMIT
		jl		invalid_input					; jump to invalid if input is too low
		cmp		cur, -1
		jg		invalid_input					; jump to invalid if input is too high
		jmp		valid_input


	; Add to the sum and recalulate the average
	valid_input:
		inc		n
		add		sum, eax
		mov		eax, sum
		CDQ
		idiv	n
		mov		av, eax
		jmp		accumulate



	; This code block executes if the user input is out of range
	; Write an error message, then prompt for input again
	invalid_input:
		cmp		cur, 0
		jge		display
		mov		edx, OFFSET string_invalid_amount
		call	WriteString
		call	Crlf
		jmp		accumulate


	; Output the amount of numbers the user entered,the sum, and the average 
	display:
		mov		eax, n
		cmp		eax, 0
		je		no_valid_numbers

		mov		edx, OFFSET string_n_1
		call	WriteString
		mov		eax, n
		call	WriteInt
		mov		edx, OFFSET string_n_2
		call	WriteString
		call	Crlf



		mov		edx, OFFSET string_sum
		call	WriteString
		mov		eax, sum
		call	WriteInt
		call	Crlf

		mov		edx, OFFSET string_av
		call	WriteString
		mov		eax, av
		call	WriteInt
		call	Crlf

		jmp		goodbye


	no_valid_numbers:
		mov		edx, OFFSET string_no_valid_numbers
		call	WriteString
		call	Crlf


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
