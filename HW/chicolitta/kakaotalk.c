#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "kakaotalk.h"

pthread_t kakaoTh_id;

void kakao(void){
    pthread_create(&kakaoTh_id, NULL, &kakaotalk, NULL);
}

void* kakaotalk(void* arg)
{
    pid_t pid;
    int status;
    printf("open API\n");
    pid = fork();

    if(pid > 0) // parent process
    {
        printf("parent\n");
        waitpid(pid, &status, 0);
    }  
    else if(pid == 0) // child process
    {
        printf("child\n");
        execl("/home/ecube/kakao.py", NULL);
        printf("fail to execute KakaoTalk\n");
    }         
    else
        printf("parent: fail to fork\n");
    
    printf("API end\n");
}


// for test
// void main(){
//	kakaotalk();
// }
