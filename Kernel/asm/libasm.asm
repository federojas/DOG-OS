GLOBAL cpuVendor
GLOBAL _checkCPUID
GLOBAL _getRTCInfo
GLOBAL _checkCPUFeatures

GLOBAL _getKey
GLOBAL _hasKey
GLOBAL _getMem
GLOBAL _exit
section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

    call _checkCPUID ;checks if processor has CPUID support 
    cmp rax, 0
    jz .end 

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

.end:
	mov rsp, rbp
	pop rbp
	ret


;https://www.youtube.com/watch?v=p5X1Sf5ejCc
_checkCPUID:
    push rbx ; save callers rbx
    pushfq ; push the flags register
    pop rax ; pop it into eax
    mov rbx, rax ; save value into ebx
    xor rax, 200000h ; set bit 21, the ID flag, to 1
    push rax ; push this toggled flags register
    popfq ; pop the toggled flags back into the flags register
    ; EN ESTE PASO EL CPU VA A RESETEAR EL BIT 21 A 0 SI NO TIENE CPUID
    pushfq ; push the flags again
    pop rax ;pop the flags into eax again
    
    cmp rax, rbx ;comapre the flags to the eax version we saved earlier
    jz No_CPUID

    pop rbx
    mov rax, 1
    ret
No_CPUID:
    pop rbx
    mov rax, 0
    ret

    
_checkCPUFeatures:
    push rbp
	mov rbp, rsp

    call _checkCPUID ;checks if processor has CPUID support
    cmp rax, 0
    jz .end

	push rdx
    push rcx
    push rbx

	mov rax, 0x1 ;GET CPU FEATURES
	cpuid

    mov [rdi], edx
    mov [rdi + 4], ecx

    xor rax, rax
    xor rcx, rcx
	mov rax, 0x7 ;GET CPU EXTENDED FEATURES
	cpuid

    mov [rdi + 8], ebx
    mov [rdi + 12], ecx

    pop rdx
    pop rcx
    pop rbx
.end:
	mov rsp, rbp
	pop rbp
	ret

_getRTCInfo:
    push rbp
    mov rbp,rsp

    mov rax, rdi
    out 70h, al
    mov rax,0
    in al, 71h

    leave
    ret

_getKey:
    push rbp
    mov rbp,rsp
    
    xor rax, rax
    in al,60h

    mov rsp, rbp
	pop rbp
    ret

_hasKey:
    push rbp
    mov rbp,rsp

    xor rax, rax
    in al,64h
    and al,0x01

    mov rsp, rbp
	pop rbp
    ret

_getMem:
    push rbp
	mov rbp,rsp

	mov al, byte[rdi]

	leave
	ret

_exit:
    jmp $