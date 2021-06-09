;
; file: quad.asm

; function quadratic
; finds solutions to the quadratic equation: 
;       a*x^2 + b*x + c = 0
; C prototype:
;   int quadratic( double a, double b, double c,
;                  double * root1, double *root2 )
; Parameters:
;   a, b, c - coefficients of powers of quadratic equation (see above)
;   root1   - pointer to double to store first root in
;   root2   - pointer to double to store second root in
; Return value:
;   returns 1 if real roots found, else 0

global  _quadratic

%define disc            qword [rbp-8]
%define one_over_2a     qword [rbp-16]

segment _DATA 
MinusFour       dw      -4


segment _TEXT
_quadratic:
        push    rbp
        mov     rbp, rsp
        sub     esp, 16         ; allocate 2 doubles (disc & one_over_2a)
        push    rbx             ; must save original ebx

        fild    word [MinusFour]; stack -4
        fld     dword[rdi]               ; stack: a, -4
        fld     dword[rdx]               ; stack: c, a, -4
        fmulp   st1             ; stack: a*c, -4
        fmulp   st1             ; stack: -4*a*c
        fld     dword[rsi]
        fld     dword[rsi]               ; stack: b, b, -4*a*c
        fmulp   st1             ; stack: b*b, -4*a*c
        faddp   st1             ; stack: b*b - 4*a*c
        ftst                    ; test with 0
        fstsw   ax
        sahf
        jb      no_real_solutions ; if disc < 0, no real solutions
        fsqrt                   ; stack: sqrt(b*b - 4*a*c)
        fstp    disc            ; store and pop stack
        fld1                   ; stack: 1.0
        fld     dword[rdi]               ; stack: a, 1.0
        fscale                  ; stack: a * 2^(1.0) = 2*a, 1
        fdivp   st1             ; stack: 1/(2*a)
        fst     one_over_2a     ; stack: 1/(2*a)
        fld     dword[rsi]               ; stack: b, 1/(2*a)
        fld     disc            ; stack: disc, b, 1/(2*a)
        fsubrp  st1             ; stack: disc - b, 1/(2*a)
        fmulp   st1             ; stack: (-b + disc)/(2*a)
        mov     ebx, dword[r10]     
        fstp    qword [ebx]     ; store in *root1
        fld     dword[rsi]               ; stack: b
        fld     disc            ; stack: disc, b
        fchs                    ; stack: -disc, b
        fsubrp  st1             ; stack: -disc - b
        fmul    one_over_2a     ; stack: (-b - disc)/(2*a)
        mov     ebx, dword[r8]
        fstp    qword [ebx]     ; store in *root2
        mov     eax, 1          ; return value is 1
        jmp     short quit

no_real_solutions:
        ffree   st0             ; dump disc off stack
        mov     eax, 0          ; return value is 0

quit:
        pop     rbx
        mov     rsp, rbp
        pop     rbp
        ret