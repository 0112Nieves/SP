#include <stdio.h>
#include <unistd.h>
int main()
{
	printf("My program starts to execute\n");
	execlp("ls", "ls", "-a", NULL);
	printf("Program running here\n");
	return 0;
}
