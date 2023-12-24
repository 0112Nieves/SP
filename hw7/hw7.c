#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#define STDIN 0
#define STDOUT 1
#define READ 0
#define WRITE 1

int main(){
    
    int pid_1,pid_2,pid_3,pfd_1[2],pfd_2[2], fd = open("the_result", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(pipe(pfd_1) == -1){
        perror(" ");
        exit(-1);
    }
    
    if((pid_1 = fork()) == -1){
        perror(" ");
        exit(-1);        
    }

    if(pid_1!=0){
        if(pipe(pfd_2) == -1){
            perror(" ");
            exit(-1);
        }
        if((pid_2=fork())==-1){
            perror(" ");
            exit(-1);
        }
        if(pid_2!=0){
            close(pfd_1[READ]);
            close(pfd_1[WRITE]);
            if((pid_3=fork())==-1){
                perror(" ");
                exit(-1);
            }
            if(pid_3!=0){
                close(pfd_2[READ]);
                close(pfd_2[WRITE]);
                wait(0);
                wait(0);
                wait(0);
            }
            else{
                close(STDIN);
                dup(pfd_2[READ]);
                close(pfd_2[READ]);
                close(pfd_2[WRITE]); 
                close(STDOUT);
                dup(fd);
                execlp("wc","wc",(char*)NULL);               
            }
        }
        else{
            close(STDIN);
            dup(pfd_1[READ]);
            close(STDOUT);
            dup(pfd_2[WRITE]);
            close(pfd_1[READ]);
            close(pfd_1[WRITE]);
            close(pfd_2[READ]);
            close(pfd_2[WRITE]);
            execlp("grep","grep","nieves",(char*)NULL);
        }
    }else{
        close(STDOUT);
        dup(pfd_1[WRITE]);
        close(pfd_1[READ]);
        close(pfd_1[WRITE]);
        execlp("ps","ps","aux",(char*)NULL);
    }
    exit(0);

    return 0;
}
