#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *myFile;
	myFile = fopen("data01.txt", "r");

	//read file into array
	int numberArray[16];
	int i;

	for (i = 0; i < 16; i++)
	{
		fscanf(myFile, "%f", &numberArray[i]);
	}

	for (i = 0; i < 16; i++)
	{
		printf("Number is: %d\n\n", numberArray[i]);
	}
}