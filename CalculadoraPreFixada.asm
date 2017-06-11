%include "io.inc"

section .data
    calc db "403 408 + 917 853 - 568 791 + 692 322 + + + 8", 0
    size EQU $-calc

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
    movsx eax, al
    PRINT_CHAR eax
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
    sub eax, '0'
    INCREMENT: 
        dec ecx
        push eax
        lodsb
        movsx eax, al
        mov edx, eax
        pop eax
        cmp edx, ' '
        je SAVE_NUM
        push edx
        mul ebx
        pop edx
        sub edx, '0'
        add eax, edx
        jmp INCREMENT

SAVE_NUM:
    mov ebx, 10
    push eax
    jmp input
    
ADD_NUM:
    pop eax
    pop edx
    add eax, edx
    jmp SAVE_NUM
    
SUB_NUM:
    pop edx
    pop eax
    sub eax, edx
    jmp SAVE_NUM
    
MUL_NUM:
    pop eax
    pop edx
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
    div ecx
    pop ecx
    jmp SAVE_NUM
    
EXP_NUM:
    pop ebx
    pop eax
    push ecx
    mov ecx, eax
    cmp ebx, 0
    je EXP_ZERO
    LOOP_EXP:
        cmp ebx, 1
        je RESTORE_ECX
        mul ecx
        dec ebx
        jmp LOOP_EXP
    EXP_ZERO:
        mov eax, 1
        jmp SAVE_NUM
        
RESTORE_ECX:
    pop ecx
    jmp SAVE_NUM