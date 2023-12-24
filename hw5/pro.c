#include <stdio.h>
#include <string.h>

int main()
{
	char str[100];
	if(fgets(str, sizeof(str), stdin) != NULL){
		str[strcspn(str, "\n")] = '\0';
		char* token = strtok(str, " ");
		while(token != NULL){
			fputs("(", stdout);
			fputs(token, stdout);
			fputs(") ", stdout);
			token = strtok(NULL, " ");
		}
		printf("\n");
	}
	else{
		printf("error input\n");
	}
	return 0;
}
