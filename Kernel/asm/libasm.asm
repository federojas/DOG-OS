GLOBAL cpuVendor
GLOBAL getRTCInfo
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

getRTCInfo:
    push rbp
    mov rbp,rsp

    mov rax, rdi
    out 70h, al
    mov rax,0
    in al, 71h

    leave
    ret
