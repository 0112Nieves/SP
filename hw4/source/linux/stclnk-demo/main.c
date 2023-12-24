#include <stdio.h>
void sayhello(void);
void change_color(int n);
int main(void)
{
	int n;
	printf("change color(1:red, 2:blue, 3:yellow, 4:green):\n");
	scanf("%d", &n);
   sayhello();
   change_color(n);
   return(0);
}

