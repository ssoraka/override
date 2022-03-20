#include <time.h>
#include <stdio.h>
#include <string.h>

void    decrypt(int key)
{
      	char	buffer[17] = "Q}|u`sfg~sf{}|a3";
        // *(int *)(&buffer) = 0x757c7d51;
        // *(int *)(&buffer + 4) = 0x67667360;
        // *(int *)(&buffer + 8) = 0x7b66737e;
        // *(int *)(&buffer + 12) = 0x33617c7d;
	      // buffer[16] = 0;
      	unsigned int	size;
      	unsigned int	i;

        size = strlen((char *)buffer); //17
        i  = 0;

	while (i < size)
	{
		buffer[i] = key ^ buffer[i];
		i += 1;
	}
	if (strncmp("Congratulations!", buffer, 17) == 0)
	{
		system("/bin/bash");
		return ;
	}
	puts("\nInvalid password");
}

void    test(int input, int value) {
        unsigned int key;

        key = value - input;
        switch (key) {
          0 : decrypt(0); break ;
          1 : decrypt(1); break ;
          2 : decrypt(2); break ;
          3 : decrypt(3); break ;
          4 : decrypt(4); break ;
          5 : decrypt(5); break ;
          6 : decrypt(6); break ;
          7 : decrypt(7); break ;
          8 : decrypt(8); break ;
          9 : decrypt(9); break ;
          10 : decrypt(10); break ;
          11 : decrypt(11); break ;
          12 : decrypt(12); break ;
          13 : decrypt(13); break ;
          14 : decrypt(14); break ;
          15 : decrypt(15); break ;
          default : decrypt(rand()); break ;
        }

}

int main(int ac, char **av) {
        int buffer;

        srand(time(NULL));
        puts("***********************************");
        puts("*\t\tlevel03\t\t**");
        puts("***********************************");
        printf("Password:");
        scanf("%d", &buffer);
        test(buffer, 0x1337d00d);
        return (0);
}
