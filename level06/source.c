//-gcc level06.c -o level06 -fno-stack-protector -z execstack -m32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>

int		auth(char *login, int serial)
{
	login[strcspn(login, "\n")] = '\0';
	int		a = strnlen(login, 32);
	//xor eax, eax ; push eax ; xor eax, eax ; je after (add 4, esp ; pop eax)
	if (a <= 5)
		return 1;
	if (ptrace(PTRACE_TRACEME, 0, (void*)1, NULL) == -1)
	{
		puts("\033[32m.---------------------------.");
		puts("\033[31m| !! TAMPERING DETECTED !!  |");
		puts("\033[32m'---------------------------'");
		return 1;
	}
	int		b = ((int)login[3] ^ 0x1337) + 0x5eeded;
	int		c = 0;
	while (a >= c)
	{
		if (login[c] <= 31)
			return 1;

		{// all variables declared here are registers, they are NOT in stack
			int	eax, edx, ecx;
			int	mul_res[2];//tmp variable, it should be stored in edx AND eax

			ecx = login[c] ^ b;
			(int64_t)*mul_res = ecx * 0x88233b2b;
			eax = (ecx - mul_res[1]) >> 1;
			eax += mul_res[1];
			eax >>= 10;
			eax *= 1337;
			edx = ecx - eax;
			b += edx;
		}
		++c;
	}
	return b != serial;
}

int		main(int ac, char **av)
{
	// 4 bytes canary
	char			buf[32];
	unsigned int	b;
	char			**a = av;//useless ?

	//xor eax, eax ; push eax ; xor eax, eax ; je after (add 4, esp ; pop eax)
	puts("***********************************");
	puts("*\t\tlevel06\t\t  *");
	puts("***********************************");
	printf("-> Enter Login: ");
	fgets(buf, 32, stdin);
	puts("***********************************");
	puts("***** NEW ACCOUNT DETECTED ********");
	puts("***********************************");
	printf("-> Enter Serial: ");
	scanf("%u", &b);
	if (auth(buf, b) == 0)
	{
		puts("Authenticated!");
		system("/bin/sh");
		//canary check
		return 0;
	}
	//canary check
	return 1;
}