GLOBAL cpuVendor
GLOBAL _checkCPUID
GLOBAL _getRTCInfo
GLOBAL _checkCPUFeatures

GLOBAL _getKey
GLOBAL _hasKey
section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

    call _checkCPUID ;checks if processor has CPUID support AGREGAR A INFORME
    cmp rax, 0 ;AGREGAR A INFORME
    jz .end  ; AGREGAR A INFORME

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


_checkCPUID:
    pushfq                               ;Save EFLAGS
    pushfq                               ;Store EFLAGS
    xor dword [rsp],0x00200000           ;Invert the ID bit in stored EFLAGS
    popfq                                ;Load stored EFLAGS (with ID bit inverted)
    pushfq                               ;Store EFLAGS again (ID bit may or may not be inverted)
    pop rax                              ;eax = modified EFLAGS (ID bit may or may not be inverted)
    xor eax,[rsp]                        ;eax = whichever bits were changed
    popfq                                ;Restore original EFLAGS
    and eax,0x00200000                   ;eax = zero if ID bit can't be changed, else non-zero
    ret
;CODIGO MODIFICADO DE https://wiki.osdev.org/CPUID PONER EN INFORME

_checkCPUFeatures:
;HACER


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
