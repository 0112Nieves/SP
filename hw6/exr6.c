#include <stdio.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define EVER ;;
void handling(int signum)
{
	int pid = fork();
	if(pid == 0) {
		execlp("uptime", "uptime", NULL);
		exit(1);
	}else {
		waitpid(pid, NULL, 0);
	}
	int pid1 = fork();
	if(pid1 == 0) {
		execlp("who", "who", NULL);
		exit(1);
	}else {
		waitpid(pid1, NULL, 0);
	}
	if(signum != SIGINT){
		alarm(10);
	}
	return;
}
int main()
{
	signal(SIGINT, handling);
	signal(SIGALRM, handling);
	alarm(10);
	for(EVER){
		pause();
	}
	return 0;
}
