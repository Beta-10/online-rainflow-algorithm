#include <stdio.h>

int a = 0;
int b, c;

int main(int argc, char **argv)
{
	printf("hello world\n");
	b = 1; c = 2;
	a = b + c;
	
	printf("The answer is: %d\r\n", a);
	return 0;
}
