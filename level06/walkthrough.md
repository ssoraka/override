The program calls the function `auth` which takes a string and int as arguments, if this function returns 1, it calls sh through `system`.
These string and int are taken from user input.
We can easily notice that the unchanged integer is compared to some key given through some encryption based on the string.
This string must be longer than 5 characters to be valid.
There is also a call to ptrace, this will require us to circumvent it within gdb.
Since the integer key is never changed in the algorithm, it only depends on the string.
We can just use gdb to examine variables during runtime and see which value it takes given some arbitrary string.
We also must not forget to disable the ptrace check by setting `eax` to 0 after the ptrace call.
Given an arbitrarily chosen input AAAAAA, we can see that the associated key is 6229050.
We can now run the program outside gdb to have to right permissions.


(gdb) b main
(gdb) r
(gdb) b *0x080487ba
(gdb) b *0x08048866
(gdb) c
Continuing.
***********************************
*		level06		  *
***********************************
-> Enter Login: BBBBBB
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 123123
(gdb) set $eax=0
(gdb) c
(gdb) x/d $ebp-0x10
0xfffed648:	6231544


level06@OverRide:~$ ./level06
***********************************
*		level06		  *
***********************************
-> Enter Login: BBBBBB
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6231544
Authenticated!
$ whoami
level07
$ cd ../level07
$ cat .pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8