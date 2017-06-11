%include "io.inc"

section .data
    calc db "$ 2 10", 0
    size EQU $-calc
    ten dd 10

section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    ;write your code here
    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx
    
    mov esi, calc
    mov ecx, size
    ; Nao contar o \0
    dec ecx
    rep lodsb
    std 
    sub esi, 1
    mov ecx, size
    sub ecx, 1
    ; Para pegar os numeros
    mov ebx, 10
    input:
        cmp ecx, 0
        je END
        dec ecx
        lodsb
        movsx eax, al
        cmp eax, ' '
        je input
        cmp eax, '0'
        jge GET_NUM
        cmp eax, '+'
        je ADD_NUM
        cmp eax, '-'
        je SUB_NUM
        cmp eax, '*'
        je MUL_NUM
        cmp eax, '/'
        je DIV_NUM
        cmp eax, '$'
        je EXP_NUM
        
    END:
    pop eax
    PRINT_DEC 4, eax
    ret

GET_NUM:
    mov ebx, 0
    sub eax, '0'
    push ecx
    INCREMENT:
        pop ecx
        dec ecx
        push eax
        lodsb
        movsx eax, al
        mov edx, eax
        pop eax
        push ecx
        mov ecx, edx
        cmp ecx, ' '
        je RESTORE_EX
        inc ebx
        sub ecx, '0'
        xchg ecx, eax
        push ebx
        MULTIPLY:
            dec ebx
            push ebx
            mov ebx, 10
            mul ebx
            pop ebx
            cmp ebx, 0
            jg MULTIPLY
        pop ebx
        xchg ecx, eax
        add eax, ecx
        jmp INCREMENT

SAVE_NUM:
    mov ebx, 10
    push eax
    jmp input
    
ADD_NUM:
    pop edx
    pop eax
    add eax, edx
    jmp SAVE_NUM
    
SUB_NUM:
    pop eax
    pop edx
    sub eax, edx
    jmp SAVE_NUM
    
MUL_NUM:
    pop edx
    pop eax
    mul edx
    jmp SAVE_NUM
    
DIV_NUM:
    pop edx
    push ecx
    mov ecx, edx
    pop edx
    pop eax
    push edx
    mov edx, 0
    xchg ecx, eax
    div ecx
    pop ecx
    jmp SAVE_NUM
    
;TODO
EXP_NUM:
    pop ebx
    pop eax
    xchg ebx, eax
    push ecx
    mov ecx, eax
    cmp ebx, 0
    je EXP_ZERO
    LOOP_EXP:
        cmp ebx, 1
        je RESTORE_EX
        mul ecx
        dec ebx
        jmp LOOP_EXP
    EXP_ZERO:
        mov eax, 1
        jmp SAVE_NUM
        
RESTORE_EX:
    pop ecx
    jmp SAVE_NUM