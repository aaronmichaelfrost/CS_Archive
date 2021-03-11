TITLE Program Template     (program1.asm)

; Author:					Aaron Frost
; Last Modified:			1/15/2021
; OSU email address:		frostaa@oregonstate.edu
; Course number/section:	CS271 001 W2021
; Assignment Number:		1							Due Date:	1/17/2021
; Description:			Outputs my name and this program title, then prompts user for two numbers from which
;						 the sum, difference, product, quotient, remainder, and squares are 
;							calculated then outputted to console

INCLUDE Irvine32.inc

.data
	string_start			BYTE	'------"Arithmetic using Assembly" by Aaron Frost------', 0

	string_ec1				BYTE	"**EC: Program verifies second input < first input", 0
	string_ec2				BYTE	"**EC: Program calculates and displays square roots of each input", 0
	
	string_prompt_1			BYTE	"Enter First Positive integer: ", 0
	string_prompt_2			BYTE	"Enter Second Positive integer: ", 0
	
	string_plus				BYTE	" + ", 0
	string_minus			BYTE	" - ", 0
	string_times			BYTE	" * ", 0
	string_over				BYTE	" / ", 0
	string_equals			BYTE	" = ", 0
	string_remainder		BYTE	" remainder ", 0

	sum						DWORD	?
	difference				DWORD	?
	product					DWORD	?
	quotient				DWORD	?
	remainder				DWORD	?

	square_1				DWORD	?
	square_2				DWORD	?

	response_1				DWORD	?
	response_2				DWORD	?

	string_end				BYTE	"Terminating Message: Goodbye!", 0

	string_invalid			BYTE	"Extra credit: Try again, second number must be less than the first!", 0
	string_pow		BYTE	" ^ ", 0





.code
main PROC

	; Introduction: outputs my name and the program title to the console
	write_start:
		mov		edx	, OFFSET string_start
		call	WriteString
		call	Crlf

		mov		edx	, OFFSET string_ec1
		call	WriteString
		call	Crlf

		mov		edx	, OFFSET string_ec2
		call	WriteString
		call	Crlf

	
	; Get data: Prompts user for integer input, then stores those integers in the response variables
	get_input:
		mov		edx, OFFSET string_prompt_1
		call	WriteString
		call	ReadInt
		mov		response_1, eax


		mov		edx, OFFSET string_prompt_2
		call	WriteString
		call	ReadInt
		mov		response_2, eax



	; EXTRA CREDIT: Validating that the second number is less than the first

	validate:
		mov		eax, response_1
		cmp		eax, response_2
		jg		calculate
		
		mov		edx, OFFSET string_invalid
		call	WriteString
		call	Crlf

		jmp		get_input




	; Calculate results: use the response variables from "Get data" section to caclulate the sum, difference, product, quotient, and remainder

	calculate:
		; Calculate & store sum
		mov		eax, response_1
		add		eax, response_2
		mov		sum, eax

		; Calculate & store difference
		mov		eax, response_1
		sub		eax, response_2
		mov		difference, eax


		; Calculate & store product
		mov		eax, response_1
		mov		ebx, response_2
		mul		ebx
		mov		product, eax


		; Calculate & store the quotient & remainder
		mov		eax, response_1
		div		response_2
		mov		quotient, eax
		mov		remainder, edx


		; EXTRA CREDIT: Calculate & store square of 1st number
		mov		eax, response_1
		mov		ebx, response_1
		mul		ebx
		mov		square_1, eax


		; EXTRA CREDIT: Calculate & store square of 2nd number
		mov		eax, response_2
		mov		ebx, response_2
		mul		ebx
		mov		square_2, eax



	; Display results: Write the numbers we calculated above to the console

	display_results:

		; Output Sum
		mov		eax, response_1
		call	WriteDec

		mov		edx, OFFSET string_plus
		call	WriteString

		mov		eax, response_2
		call	WriteDec

		mov		edx, OFFSET string_equals
		call	WriteString

		mov		eax, sum
		call	WriteDec

		call	Crlf		



		; Output Difference
		mov		eax, response_1
		call	WriteDec

		mov		edx, OFFSET string_minus
		call	WriteString

		mov		eax, response_2
		call	WriteDec

		mov		edx, OFFSET string_equals
		call	WriteString

		mov		eax, difference
		call	WriteDec

		call	Crlf	




		; Output Product
		mov		eax, response_1
		call	WriteDec

		mov		edx, OFFSET string_times
		call	WriteString

		mov		eax, response_2
		call	WriteDec

		mov		edx, OFFSET string_equals
		call	WriteString

		mov		eax, product
		call	WriteDec

		call	Crlf	



		; Output Quotient & Remainder
		mov		eax, response_1
		call	WriteDec

		mov		edx, OFFSET string_over
		call	WriteString

		mov		eax, response_2
		call	WriteDec

		mov		edx, OFFSET string_equals
		call	WriteString

		mov		eax, quotient
		call	WriteDec

		mov		edx, OFFSET string_remainder
		call	WriteString

		mov		eax, remainder
		call	WriteDec
		call	Crlf	




		; Output square of first input
		mov		eax, response_1
		call	WriteDec

		mov		edx, OFFSET string_pow
		call	WriteString

		mov		eax, 2
		call	WriteDec

		mov		edx, OFFSET string_equals
		call	WriteString

		mov		eax, square_1
		call	WriteDec

		call	Crlf	


		; Output square of second input
		mov		eax, response_2
		call	WriteDec

		mov		edx, OFFSET string_pow
		call	WriteString

		mov		eax, 2
		call	WriteDec

		mov		edx, OFFSET string_equals
		call	WriteString

		mov		eax, square_2
		call	WriteDec

		call	Crlf	


	; Write a terminating message
	write_end:
		mov		edx, OFFSET string_end
		call	WriteString


	exit	; exit to operating system
main ENDP


; (insert additional procedures here)

END main
