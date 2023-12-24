#include <stdio.h>
void change_color(int n)
{
	switch(n){
		case 1:
			printf("\033[1;31msuccess\033[0m\n");
			break;
		case 2:
			printf("\033[1;34msuccess\n\033[0m");
			break;
		case 3:
			printf("\033[1;33msuccess\n\033[0m");
			break;
		case 4:
			printf("\033[1;32msuccess\n\033[0m");
			break;
		default:
			printf("Error, please between 1~4.\n");
			break;
	}
}
