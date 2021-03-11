TITLE Program Template     (program5.asm)

; Author:					Aaron Frost
; Last Modified:			2/25/2021
; OSU email address:		frostaa@oregonstate.edu
; Course number/section:	CS271 001 W2021
; Assignment Number:		5							Due Date:	2/28/2021
; Description:			Requests user for a number in the range [min, max], then generates that many random numbers in the range [lo, hi]. Prints the original array to console, displays the median to console, 
;						 sorts the array in descending order using bubble sort, then prints the sorted array to console













INCLUDE Irvine32.inc

.data

	lo = 100
	hi = 999

	min = 15
	max = 200

	max_array_size = 10000

	row_len = 10


	string_start			BYTE	"~~~~ Sorting Random Numbers by Aaron Frost ~~~~", 0

	string_desc				BYTE	"This program generates random numbers in the range [", 0

	string_comma			BYTE	", ", 0

	string_desc_1			BYTE	"], then displays the original list, sorts the list, then calculates and displays the median value. Finally, it displays the list sorted in descending order.", 0

	string_end				BYTE	"This program will now terminate. Goodbye!", 0

	string_prompt			BYTE	"Enter the amount of random numbers to generate in the range [", 0

	string_prompt_1			BYTE	"]: ", 0

	string_title_sorted		BYTE	"Sorted Array: ", 0

	string_title_original	BYTE	"Randomly Generated Array: ", 0

	
	; The length of the array to generate 
	request					DWORD	?

	array					DWORD	max_array_size		DUP(0)

	
	string_bad_input		BYTE	"Out of range, try again.", 0

	string_median			BYTE	"The median after sorting is ", 0

	string_space			BYTE	"   ", 0



	



.code
main PROC


	; Output introduction and program description
	call	intro



	; Get valid input for the amount of random numbers to generate
	push	OFFSET request
	call	get_data



	; Fill the array with (request) random numbers in the range of [lo, hi]
	push	OFFSET array
	push	request
	call	fill_array



	; Display the unsorted and random array
	push	OFFSET array
	push	request
	push	OFFSET string_title_original
	call	print_array



	; Sort the array in descending order
	push	OFFSET array
	push	request
	call	sort_descending



	; Display the median 
	push	OFFSET array
	push	request
	call	display_median



	; Display the sorted array
	push	OFFSET array
	push	request
	push	OFFSET string_title_sorted
	call	print_array



	; Output goodbye text
	call	outro


	exit	; exit to operating system
main ENDP









; sort_descending

; Description: sorts an integer array (DWORD) passed by reference in descending order using bubble sort

; Inputs: push the offset of the array, then push the length of the array by value
; Outputs: sorts the array in descending order

sort_descending PROC

	enter	0,0


	; Save registers
	push	ebx
	push	eax
	push	ecx
	push	esi


	; Initialize loop counter with request
	mov		ecx, [ebp + 8]


	; Decrement parameter and ecx once. We are going to use the paramenter as an indexer for bubble sorting
	dec		ecx
	mov		[ebp + 8], ecx


	; Initialize the array indexer
	mov		esi, [ebp + 12]



; Make a comparison and swap values if neccecary
compare:
	mov		eax, [esi]
	mov		ebx, [esi + TYPE DWORD]


	; Swap when the first element is less than the second
	cmp		eax, ebx
	jl		exchange


; Perform the next iteration or complete the pass
next:
	add		esi, TYPE DWORD
	loop	compare
	jmp		pass_complete



exchange:
	mov		[esi], ebx
	mov		[esi + TYPE DWORD], eax
	jmp		next



; We've finished the first pass. Next pass we will need to make 1 less comparison. If we need to make zero comparisons ([ebp + 8] == 0), then the sort is complete. If not, decrement [ebp + 8] and start the next pass
pass_complete:

	mov		ecx, [ebp + 8]
	dec		ecx
	cmp		ecx, 0
	je		done

	mov		[ebp + 8], ecx
	mov		esi, [ebp + 12]
	jmp		compare


; Sorting is complete, restore registers and return
done:
	pop		esi
	pop		ecx
	pop		eax
	pop		ebx

	leave
	ret		8


sort_descending ENDP







; display_median

; Description: outputs the median of an integer array to the console

; Inputs: push the offset of the array, then push the length of the array by value
; Outputs: outputs median of array to console

display_median PROC

	enter	0,0

	push	edx
	push	eax
	push	ebx
	push	ecx
	push	esi


	mov		edx, OFFSET string_median
	call	WriteString



	; Initialize loop counter
	mov		ecx, [ebp + 8]
	

	; Initialize array indexer
	mov		esi, [ebp + 12]


	; Divide length by two
	mov		eax, ecx
	mov		edx, 0		; clear edx for division
	mov		ebx, 2
	div		ebx

	push	edx

	


	; Scale by type
	mov		ebx, TYPE DWORD
	mul		ebx


	; Set eax to the center element
	add		esi, eax
	mov		eax, [esi]


	pop		edx
	cmp		edx, 0

	je		case_even

	jmp		done


case_even:

	mov		ebx,  [esi - TYPE DWORD]

	add		eax, ebx

	mov		ebx, 2

	mov		edx, 0

	div		ebx


	; Round up if neccecary
	cmp		edx, 0

	je		done

	inc		eax




; Print eax, the median, restore registers, then return
done:

	call	WriteDec
	call	Crlf


	pop		esi
	pop		ecx
	pop		ebx
	pop		eax
	pop		edx

	leave

	ret		8

display_median ENDP








; print_array
;
; Description: prints each element the array after printing the title of the array, assumes array elements are DWORD
;
; Inputs: Push arguments in this order:
;			1. offset of array
;			2. number of elements
;			3. offset of title string
;
; Outputs: Outputs the title and the array to console


print_array PROC

	enter	0,0
	

	mov		esi, [ebp + 16]
	mov		ecx, [ebp + 12]

	; Leave if array is empty
	cmp		ecx, 0
	je		done

	; Write the title
	mov		edx, [ebp + 8]
	call	WriteString
	call	Crlf

	; Initialize the row length counter
	mov		ebx, 0

print_element:

	; Print the element
	mov		eax, [esi]
	call	WriteDec

	; Create a new row if needed
	inc		ebx
	cmp		ebx, row_len
	je		new_line

	; Add a space
	mov		edx, OFFSET string_space
	call	WriteString

	jmp		next


new_line:
	mov		ebx, 0
	call	Crlf
	jmp		next

next:
	add		esi, TYPE DWORD
	loop	print_element

done:
	call	Crlf
	leave
	ret		12




print_array ENDP








; fill_array

; Description: fills an array passed by reference with integers randomly generated within the range [lo, hi]

; Inputs: push the offset of the array, then push the length of the array by value
; Outputs: fills the array passed in with random integers

fill_array PROC

	call	Randomize

	enter	4,0

	mov		esi, [ebp + 12]
	mov		ecx, [ebp + 8]

	cmp		ecx, 0
	je		L2
	

	; Set the range to use for random number generation
	mov		ebx, hi
	sub		ebx, lo
	inc		ebx
	mov		DWORD PTR [ebp - 4], ebx



L1:
	mov		eax, [ebp - 4]
	call	RandomRange
	add		eax, lo
	mov		[esi], eax
	add		esi, TYPE DWORD
	loop	L1


L2:
	leave
	ret		8

fill_array ENDP







; intro

; Description: writes the intro text to console

; Inputs: none
; Outputs: outputs text to console
intro PROC

	push	edx
	push	eax

	mov		edx, OFFSET string_start
	call	WriteString
	call	Crlf

	mov		edx, OFFSET string_desc
	call	WriteString

	mov		eax, lo
	call	WriteDec

	mov		edx, OFFSET string_comma
	call	WriteString

	mov		eax, hi
	call	WriteDec

	mov		edx, OFFSET string_desc_1
	call	WriteString


	call	Crlf

	pop		eax
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


	enter	0,0


	; Save registers
	push	eax
	push	edx



prompt:
	mov		edx, OFFSET string_prompt
	call	WriteString

	mov		eax, min
	call	WriteDec


	mov		edx, OFFSET string_comma
	call	WriteString

	
	mov		eax, max
	call	WriteDec

	mov		edx, OFFSET string_prompt_1
	call	WriteString



; Store user input
get_input:
	call	ReadDec

	cmp		eax, min
	jl		bad_input
	cmp		eax, max
	jg		bad_input

	jmp		good_input



; Retry if input is not in range
bad_input:
	mov		edx, OFFSET string_bad_input
	call	WriteString
	call	Crlf
	jmp		prompt


good_input:
	call	Crlf


	mov		edx, [ebp + 8]
	
	mov		[edx], eax

	; Restore registers
	pop		edx
	pop		eax

	leave

	ret		4

get_data ENDP



END main