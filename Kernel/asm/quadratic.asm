; Ecuacion cuadratica

%define a               qword [rdi]
%define b               qword [rsi]
%define c               qword [rdx]

global _quadratic

section .bss
disc resq 8
uno_sobre_2a resq 8


section .data 
minusFour       dw      -4

section .text

;codigo modificado de https://github.com/humanshell/assembly/blob/master/examples/borland/quad.asm
_quadratic:
    push rbp
    mov rbp, rsp

    push rbx
    push rcx
                        
    fild word[minusFour] ; -4
    fld a               ; a, -4
    fld c               ; c, a, -4
    fmulp st1           ; ac, -4
    fmulp st1           ;   -4ac
    fld b                
    fld b
    fmulp st1           ; bb, -4ac
    faddp st1           ; bb - 4a*c


    fsqrt               ; sqrt(bb - 4a*c)
    fstp qword[disc]    ; bss sqrt(bb - 4a*c), vacio stack
    fld1                ; 1
    fld a               ; a, 1
    fscale              ; a * 2^1 = 2*a, 1
    fdivp st1           ; 1/2a
    fst qword[uno_sobre_2a] ; bss 1/2a, no vacio stack

    fld b               ; b, 1/2a
    fld qword[disc]     ; sqrt(bb - 4a*c), b, 1/2a
    fsubrp st1          ; sqrt(bb - 4a*c) - b, 1/2a
    fmulp st1           ; sqrt(bb - 4a*c) - b * 1/2a
    fstp qword[r8]     ; guardo raiz1 y vacio stack 

    fld b               ; b
    fld qword[disc]     ; sqrt(bb - 4a*c), b
    fchs                ; -sqrt(bb - 4a*c), b
    fsubrp st1          ; -sqrt(bb - 4a*c) - b
    fmul qword[uno_sobre_2a] ; -sqrt(bb - 4a*c) - b * 1/2a
    fstp qword[r9] ; guardo raiz2 y vacio stack 

    mov rax, 1

    pop rcx
    pop rbx
    
    mov rsp, rbp
    pop rbp
    ret