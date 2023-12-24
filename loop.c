#include <stdio.h>
#include <unistd.h>
int main()
{
	for(int i = 1; i < 10000; i++){
		printf("Hello! Thanks i=%d\n", i);
		sleep(1);
	}
	return 0;
}
