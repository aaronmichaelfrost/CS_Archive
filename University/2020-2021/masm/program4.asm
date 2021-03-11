TITLE Program Template     (program4.asm)

; Author:					Aaron Frost
; Last Modified:			2/11/2021
; OSU email address:		frostaa@oregonstate.edu
; Course number/section:	CS271 001 W2021
; Assignment Number:		4							Due Date:	2/14/2021
; Description:			Recieves integer input n, outputs n consecutive composite numbers starting at 4


INCLUDE Irvine32.inc

.data

	LOW_LIMIT = 1
	HIGH_LIMIT = 300
	MAX_PER_ROW = 10

	string_start			BYTE	"~~~~ Composite Numbers by Aaron Frost ~~~~", 0

	string_desc				BYTE	"This program generates a list of composite numbers, starting at 4.", 0

	string_end				BYTE	"This program will now terminate. Goodbye!", 0

	string_prompt_1			BYTE	"Enter the amount of composite numbers to generate (Range[ ", 0

	string_prompt_2			BYTE	", ", 0

	string_prompt_3			BYTE	"]): ", 0

	string_bad_input		BYTE	"Out of range, try again.", 0

	string_space			BYTE	"   ", 0

	current_col				DWORD	0

	composites_shown		DWORD	0

	n						DWORD	?

	composite_flag			BYTE	0

	valid_flag				BYTE	0

	temp					DWORD	?


.code
main PROC


	; Output introduction and program description
	call	intro

	; Get a valid input for number of composites to show (n)
	call	get_data

	; Iterate thorugh natural numbers until we have outputted n composites
	call	show_composites

	; Output goodbye text
	call	outro


	exit	; exit to operating system
main ENDP




; intro

; Description: writes the intro text to console

; Inputs: none
; Outputs: outputs text to console
intro PROC

	push	edx

	mov		edx, OFFSET string_start
	call	WriteString
	call	Crlf

	mov		edx, OFFSET string_desc
	call	WriteString
	call	Crlf

	pop		edx
		
	ret 
;
intro ENDP



; outro

; Description: writes the outro text to console

; Inputs: none
; Outputs: outputs text to console
outro PROC

	push	edx

	call	Crlf

	mov		edx, OFFSET string_end
	call	WriteString
	call	Crlf

	pop		edx
		
	ret 

outro ENDP



; get_data

; Description: sets the value of n to user input, calls validate

; Inputs: none
; Outputs: sets global variable n to an integer in a set range

get_data PROC

; Save registers on stack
	push	edx
	push	eax

; Ask user for how many composite numbers they want to see (within range)
prompt:
	mov		edx, OFFSET string_prompt_1
	call	WriteString

	mov		eax, LOW_LIMIT
	call	WriteDec


	mov		edx, OFFSET string_prompt_2
	call	WriteString

	
	mov		eax, HIGH_LIMIT
	call	WriteDec

	mov		edx, OFFSET string_prompt_3
	call	WriteString

	jmp		get_input


; Store user input
get_input:
	call	ReadDec

	call	validate



	cmp		valid_flag, 1
	je		set_n



; Retry if input is not in range
bad_input:
	mov		edx, OFFSET string_bad_input
	call	WriteString
	call	Crlf
	jmp		prompt


set_n:
	mov		n, eax
	call	Crlf


; Restore registers
	pop		eax
	pop		edx

	ret 
;
get_data ENDP





; validate

; description: sets the valid_flag based on whether eax is between LOW_LIMIT and HIGH_LIMIT

; Inputs: eax (the number to validate)
; Outputs: sets valid_flag to 1 or 0

validate PROC


	cmp		eax, LOW_LIMIT
	jl		return_false
	cmp		eax, HIGH_LIMIT
	jg		return_false


return_true:
	mov		valid_flag, 1
	jmp		return

return_false:
	mov		valid_flag, 0



return:

	ret
validate ENDP




; show_composites

; description: writes a list of n consecutive composite numbers

; Inputs: requires that n is set to the number of composites to be displayed
; Outputs: prints n consecutive composite numbers to console

show_composites PROC

; Save registers on the stack
	push	edx
	push	eax

; Start the loop at four. Everyone knows that 2 is composite I guess. (This is what the example showed to do)
	mov		eax, 4


; Keep checking if numbers are composite until we have printed n composites
L1:
	mov		ebx, composites_shown
	cmp		ebx, n
	je		return

	call	is_composite


	cmp		composite_flag, 1
	je		print



next:
	inc		eax
	jmp		L1
	jmp		return



; Print a composite number
print:
	call	WriteDec
	mov		edx, OFFSET string_space
	call	WriteString

	inc		composites_shown
	inc		current_col
	
	
	cmp		current_col, MAX_PER_ROW
	je		new_row

	jmp		next


; Start a new row
new_row:
	call	Crlf
	mov		current_col, 0
	jmp		next



return:
; Restore registers to their original values
	pop		eax
	pop		edx
		
	ret 
;
show_composites ENDP




; is_composite

; Description: Sets the composite flag if the value in eax is composite

; Input: eax
; Output: composite_flag
is_composite PROC

	; Save registers on stack
	push	ebx
	push	eax
	push	ecx


	mov		ebx, 2
	mov		composite_flag, 0
	mov		temp, eax



	; Number of iterations is eax - 2

	sub		eax, 2

	mov		ecx, eax


; Psuedo-code

; for( ebx = 2; ebx < temp; ebx++)
;	if(temp % ebx == 0) 
;		return true;

check_remainder:
	cmp		ebx, temp
	je		return_false


	mov		edx, 0
	mov		eax, temp
	div		ebx


	cmp		edx, 0
	je		return_true

	inc		ebx
	loop	check_remainder



return_false:
	mov		composite_flag, 0
	jmp		return


return_true:
	mov		composite_flag, 1


return:

	; Restore registers
	pop		ecx
	pop		eax
	pop		ebx


	ret
is_composite ENDP


END main