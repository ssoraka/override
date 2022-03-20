#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	char	buffer[0x64]; //100
	int	size;

	size = 0;
	fgets(buffer, 0x64, stdin);
	size = 0;
	while (size < strlen(buffer))
	{
		if (buffer[size] > 0x40) { //64
			if (buffer[size] <= 0x5a) { //90
				buffer[size] = buffer[size] ^ 0x20; //0010 0000
			}
		}
		printf("\\x%02hhx", buffer[size]);
		size += 1;
	}
	printf("\n");
	printf(buffer);
	exit(0);
}
