#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define ERR (-1)
#define READ 0
#define WRITE 1
#define STDIN 0
#define STDOUT 1
int main()
{
	int pid_1, pid_2, pfd[2];
	if(pipe(pfd) == ERR){
		perror(" ");
		exit(ERR);
	}
	if((pid_1 = fork()) == ERR){
		perror(" ");
		exit(ERR);
	}
	if(pid_1 != 0){
		printf("\npdf[0]=%d, pdf[1]=%d.\n", pfd[0], pfd[1]);
		if((pid_2 = fork()) == ERR){
			perror(" ");
			exit(ERR);
		}
		if(pid_2 != 0){
			close(pfd[READ]);
			close(pfd[WRITE]);
			wait((int *)0);
			wait((int *)0);
		}
		else{
			close(STDIN);
			dup(pfd[READ]);
			close(pfd[READ]);
			close(pfd[WRITE]);
			execl("/usr/bin/wc", "ls", (char *) NULL);
		}
	}
	else{
		close(STDOUT);
		dup(pfd[WRITE]);
		close(pfd[READ]);
		close(pfd[WRITE]);
		execl("/usr/bin/who", "ls", (char *)NULL);
	}
	printf("pid1 is %d pid2 is %d\n",pid_1,pid_2);
	//exit(0);
	return 0;
}
