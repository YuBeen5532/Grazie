#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "kakaotalk.h"

pthread_t kakaoTh_id;

/*======================= 카카오톡 thread 생성 ==============================*/
void kakao(void){
    pthread_create(&kakaoTh_id, NULL, &kakaotalk, NULL);
}
/*===========================================================================*/

/*======================= 카카오톡 thread 함수 ==============================*/
void* kakaotalk(void* arg)
{
    pid_t pid;
    int status;
    printf("open API\n");

    pid = fork();

    // parent process
    if(pid > 0) 
    {
        printf("parent\n");
        // child process 기다림
        waitpid(pid, &status, 0);
    }  
    // child process
    else if(pid == 0) 
    {
        printf("child\n");

        // exec함수를 사용하여 카카오톡 api를 위한 프로그램 실행
        execl("/home/ecube/kakao.py", NULL);
        printf("fail to execute KakaoTalk\n");
    }         
    else
        printf("parent: fail to fork\n");
    
    printf("API end\n");
}
/*===========================================================================*/


