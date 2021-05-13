GLOBAL cpuVendor
GLOBAL getSecondsAsm
GLOBAL getMinutesAsm
GLOBAL getHoursAsm
section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getSecondsAsm:
	push rbp
	mov rbp, rsp 

	xor rax, rax 
	mov al, 0 
	out 70h, al 
	xor rax, rax 
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

getHoursAsm:
	push rbp
	mov rbp, rsp 

	xor rax, rax 
	mov al, 4 
	out 70h, al 
	xor rax, rax 
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

getMinutesAsm:
	push rbp
	mov rbp, rsp 

	xor rax, rax 
	mov al, 2 
	out 70h, al 
	xor rax, rax 
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

