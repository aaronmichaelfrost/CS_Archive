TITLE Final Project     (finalprogram.asm)

; Author:					Aaron Frost
; Last Modified:			3/11/2021
; OSU email address:		frostaa@oregonstate.edu
; Course number/section:	CS271 001 W2021
; Assignment Number:		final (6)							Due Date:	3/17/2021
; Description:			implements three enumerable functions within the compute function: 
;							1. decoy - calculates the sum of two signed WORDs and returns the sum at a paramterized offset
;							2. encrypt - replaces characters at a parameterized offset string using a key mapped 1-1 with the 26 character alphabet
;							3. decrypt - replaces characters at a parameterized offset string using the original characters that correspond to indices of 
;										mapped characters from the key


INCLUDE Irvine32.inc


.code
main PROC


	exit
main ENDP



;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
; compute
;
;
; Description: three modes, specified by dest parameter: decoy(0), encryption(-1), and decryption(-2)
;
;
; Inputs:
;
;	For decoy, push operand1 (WORD) then push operand2 (WORD), then push OFFSET dest (DWORD)
;
;	For encryption && decryption, push OFFSET key (string) then push OFFSET message (string), then push OFFSET dest (DWORD)
;
;
; Outputs:
;
;	For decoy, sum of operand1 and operand2 is stored at OFFSET dest
;
;	For encryption, the encrypted message is stored at OFFSET message
;
;	For decryption, the decrypted message is stored at OFFSET message
;
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
compute PROC

	enter	0,0

	pushad


	; Move dest to eax to choose the mode
	mov		eax, [ebp + 8]

	mov		eax, [eax]



	cmp		eax, 0
	je		decoy_mode

	cmp		eax, -1
	je		encryption_mode

	cmp		eax, -2
	je		decryption_mode



decoy_mode:

	; push operand 1 and 2
	push	[ebp + 12]

	; push OFFSET dest
	push	[ebp + 8]

	call	decoy

	jmp		done


encryption_mode:

	; push key offset
	push	[ebp + 16]

	; push message offset
	push	[ebp + 12]


	call	encrypt

	jmp		done


decryption_mode:


	; push key offset
	push	[ebp + 16]

	; push message offset
	push	[ebp + 12]


	call	decrypt

	jmp		done


done:
	popad
	leave
	ret		12


compute ENDP







;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
; decrypt
;
;
; Description: replaces encrypted letters with the true characters that exist at the corresponding index in the key
;
;
; Inputs:
;
;	push OFFSET key (string) then push OFFSET message (string)
;
;
; Outputs:
;
;	stores decrypted message at OFFSET message
;
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
decrypt PROC


	enter	0,0


	; initialize the indexer at start of message
	mov		esi, [ebp + 8]


foreach_original:


	mov		eax, [esi]

	cmp		al, 0
	je		done

	cmp		al, 97
	jl		next_original

	cmp		al, 122
	jg		next_original


	; Initialize loop counter
	mov		ecx, 0




foreach_replacement:
	
	; EDX gets the key
	mov		ebx, [ebp + 12]

	add		ebx, ecx

	mov		ebx, [ebx]

	cmp		bl, al

	jne		next_replacement

	; If replacement == original, original = real_alphabet[ecx]

	add		ecx, 97

	mov		[esi], cl


next_replacement:

	inc		ecx

	add		ebx, TYPE BYTE

	cmp		ecx, 26


	jg		next_original

	jmp		foreach_replacement



next_original:

	add		esi, TYPE BYTE

	jmp		foreach_original


done:

	leave

	ret		8


decrypt ENDP







;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
; encrypt
;
;
; Description: replaces original letters with the letters mapped by the key
;
;
; Inputs:
;
;	push OFFSET key (string) then push OFFSET message (string)
;
;
; Outputs:
;
;	stores encrypted message at OFFSET message
;
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
encrypt PROC


	enter	0,0


	; initialize the indexer at start of message
	mov		esi, [ebp + 8]


L1:

	; End the loop once we reach the end of the message

	mov		eax, [esi]

	cmp		al, 0
	je		done

	cmp		al, 97
	jl		next

	cmp		al, 122
	jg		next

	

	; Al gets key index
	sub		al, 97

	; EDX gets the key
	mov		edx, [ebp + 12]

	; EAX gets index (ASCII of original - 97)
	AND		eax, 000000FFh

	; EDX gets replacement character offset (edx = address of replacement character)
	add		edx, eax

	; EAX gets replacement character value
	mov		eax, [edx]

	; Character is replaced
	mov		[esi], al



next:

	add		esi, TYPE BYTE

	jmp		L1


done:

	leave

	ret		8


encrypt ENDP









;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
; decoy
;
;
; Description: adds two numbers
;
;
; Inputs:
;
;	push operand1 (WORD) and operand2 (WORD), then push OFFSET dest (DWORD)
;
;
; Outputs:
;
;	stores sum in dest
;
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
decoy PROC

	enter	0,0

	mov		ax, [ebp + 14]
	movsx	eax, ax

	mov		bx, [ebp + 12]
	movsx	ebx, bx


	add		eax, ebx


	mov		ebx, [ebp + 8]
	mov		[ebx], eax
 
	leave

	ret		8

decoy ENDP





END main