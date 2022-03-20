# Level3

We can see that the program take an int as input, try to decrypt a key with that input, and either call `system("/bin/sh");`, or print an error message.
Despite the weird code in the test function, we conclude by testing that the decrypt() func is always called.

the algorithm in the decrypt function is just a XOR between a constant string and our input (after modification through the test function).
Once we see that the algorithm xor encrypts an existing string before comparing it to "Congratulations!", we simply need to find the string it is comparing it to.
Using a simple breakpoint before the strcmp, we see that it points to "Q}|u`sfg~sf{}|a3"

Now we just find the key such that Q ^ x = C, we find out that it equals 18:
 01010001 'Q'
^00010010 18
=01000011 'C'

The key is set to be `0x1337d00d` minus the user input, we want it to be equal to 18.
Our input should then be `0x1337cffb`, that is, 322424827 in decimal.

level03@OverRide:~$ ./level03
***********************************
*        level03        **
***********************************
Password:322424827
$ whoami
level04
$ cd ../level04
$ cat .pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf