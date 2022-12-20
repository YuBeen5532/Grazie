#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/kd.h>
#include <linux/input.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/msg.h>
#include "../libfbdev/libfbdev.h"
#include "../libjpeg/jpeglib.h"
#include "display.h"
#include "buzzer.h"
#include "button.h"
#include "accelMagGyro.h"
#include "led.h"
#include "temperature.h"
#include "colorLed.h"
#include "fnd.h"
#include "textlcd.h"
#include "kakaotalk.h"
#include "tting.h"

#define MAX_SCALE_STEP 8 

/*================================= 전역변수 ==========================================*/
pthread_t walkTh_id, countTh_id;
float temp;
int life=8;
int pre_life=8;
int hungry=0;
int thirsty=0;
int dirty=0;
int hour=6;
int day = 1;
const int MusicScale[8] =
{
    262, /*do*/ 294,330,349,392,440,494, /* si */ 523
};
/*====================================================================================*/

/*================================= 함수원형 ==========================================*/
void* countTh(void* arg); 
void feed_Func(int lv_state);
void water_Func(int lv_state);
void clean_Func(int lv_state);
int inven_Func(int lv_state);
void minigame_Func(void);
int update_state(int lv_state);
/*====================================================================================*/

/*================================= main 함수  ==================================*/
int main(void)
{
    accInit();              //Accel
    FindBuzzerSysPath();    //Buzzer
    pwmLedInit();           //colorLED
    displayInit();          //TFT-LCD
    buttonInit();           //Button
    ledInit();              //LED
    fndInit();              //FND
    textlcdInit();          //TXT-LCD

    while(1){
        int pick_state=pick_feed;  // display에 pick_state를 보냄 

        int lv_state=lv_baby;     // 기본값: 새싹
        int pick_button;
        print_display_up(title);  // 처음에 title화면 출력
        usleep(100000);
        fnd_hour(hour , day);
        ledLife(life);
        levelwrite(lv_state);
        tempwrite(temp);

        while(1)
        {
            pick_button = button_rcv();                   // 버튼의 입력받아 
            if(pick_button == pick_btn_exit) break;       // 홈key가 눌렸다면 => while문 탈출, 게임시작
        }    
        
        print_display_up(lv_clova);                       // 게임시작되면 새싹화면 출력
        print_display_down(pick_no);
        sleep(1);
    
        while(1){
            printf("hungry : %d\n\r",hungry);
            printf("thirsty : %d\n\r",thirsty);
            printf("dirty : %d\n\r",dirty);
            printf("life : %d\n\r",life);
            printf("day : %d\n\r",day);
            printf("hour : %d\n\r",hour);
            printf("temp : %f\n\r",temp);

            pthread_create(&countTh_id, NULL, &countTh, NULL);  // 카운트 thread 생성

            while(1)
            {        
                print_display_up(lv_state);      // 현재 level의 띵코리타 모습 
                print_display_down(pick_state);  // 현재 display상에서의 선택        
                usleep(100000);                  // 화면이 깨지지 않도록 delay
                fnd_hour(hour , day);            // 인게임 일차, 시간을 fnd에 출력

                pick_button = 0;
                pick_button = button_rcv();      // 어떤 행동할지 고르는 버튼 입력

                if(pick_button == pick_btn_left) // 왼쪽 선택지로 이동
                 {
                    if(pick_state>=5) pick_state -= 1; 
                    else if(pick_state==4) pick_state = 8;
                 }
                if(pick_button == pick_btn_right) // 오른쪽 선택지로 이동
                 {
                    if(pick_state<=7) pick_state += 1;
                    else if(pick_state==8) pick_state = 4;
                 }
                if(pick_button == pick_btn_lv)    // 데모 시 레벨업을 빨리하기 위한 버튼
                 {
                    if((lv_state>=2 && lv_state<=6) || (lv_state>=14 && lv_state<=16)) lv_state += 1; // 자켓 입었을 경우 & 자켓 입지않은 경우
                 }

                if(pick_button == pick_btn_exit)
                 {
                    if(lv_state == lv_baby && (pick_state == pick_walk || pick_state == pick_clean || pick_state == pick_inven )) continue; // 처음 아기 단계일 때 물주기 밥주기 이외의 선택했을 시에 다시 버튼 입력을 받음
                    else break;
                 }
                if(pick_button == timeout) break; // 버튼 입력받는 시간 초과시 바로 상태변경
            }

        COUNT_MSG_T Count;
        int countID = msgget (COUNT_MESSAGE_ID, IPC_CREAT|0666);
        Count.count_msg = 10;                           // 버튼이 입력되면  Count.count_msg 에 10의 값을 주어 count thread가 정지되도록                                    
        msgsnd(countID, &Count, sizeof(Count)-4, 0);    // Count.count_msg==10 임을 알리기 위한 메세지

        if(pick_button != timeout){                     // timeout상태가 아닐 때 행동 반영
            switch(pick_state)                          
            {
                case 4: feed_Func(lv_state);  break;                // 밥주기
                case 5: water_Func(lv_state); break;                // 물주기
                case 6: minigame_Func(); break;             // 산책하기
                case 7: clean_Func(lv_state); break;                // 청소하기
                case 8: lv_state = inven_Func(lv_state); break;     // 인벤토리 
            }
         }
        lv_state = update_state(lv_state);   // 상태변경 반영


        if((lv_state % 10 == 7) && (hour == 6)) { print_display_up(happy_ending); break;} // 최고 레벨에 도달하고, 인게임 시간이 AM 6:00이면 해피엔딩으로 게임종료
        if(life<=0){print_display_up(sad_ending); break;}                                 // 체력이 0이 되면 새드엔딩으로 게임종료
        }

        while(1)
        {
             // 게임이 종료되고 다시 버튼입력이 되면 TITLE화면으로 돌아가 게임 새로 시작
            pick_button = button_rcv();                   
            if(pick_button == pick_btn_exit) break;       
        }
        
        // 새로 게임을 시작할 때 값을 초기화
        life=8;
        pre_life=8;
        hungry=0;
        thirsty=0;
        dirty=0;
        hour=6;
        day = 1;
    }

    displayExit();
    buttonExit();
    fndExit();
    textlcdExit();
    buzzerExit();
    ledExit();
}

/*====================================================================================================*/


/*======================================= 인벤토리 ====================================================*/
int inven_Func(int lv_state){ 

   int inven_state=inven_wear;          // 인벤토리에 들어가면 착용에 O표시로 선택되어 있도록
   print_display_up(inven_state);

   usleep(100000);
   int select=0;   
         
   while(1)
   {
      select=button_rcv();

     // left 버튼 누른 경우
      if(select==3){                   
         if(inven_state==inven_wear)
            inven_state=inven_unwear;
         else 
            inven_state=inven_wear;
         print_display_up(inven_state);
      }
     // right 버튼 누른 경우
      else if(select==4){                
         if(inven_state==inven_unwear) 
            inven_state=inven_wear;
         else
            inven_state=inven_unwear;
         print_display_up(inven_state);
      }
      // OK 버튼 누르면 while문 탈출
      else if(select == 5) break;
   }

   // 착용 선택에 따른 lv_state 변경
   if(inven_state==inven_wear && lv_state<10)
      lv_state=lv_state+10;   
   // 해제 선택에 따른 lv_state 변경
   else if(inven_state==inven_unwear && lv_state>10)
      lv_state = lv_state-10;   
   
   print_display_up(lv_state);
   usleep(100000);
      
   return lv_state;
}
/*====================================================================================================*/

/*======================================= 미니게임 ====================================================*/
void minigame_Func(){

	int walk_state=walk_center;  // 산책시 띵코리타의 위치 기본값: 중앙 
    DISPLAY_MSG_T DeadSignal;   // 장애물이 어느 위치에 있는지 알리는 메세지(미니게임이 받는 메세지 -> display.c의 미니게임 thread참고 )
    DISPLAY_MSG_T DeadFlag;     // 띵코리타와 장애물이 닿았을 시 알리는 메세지(미니게임이 보내는 메세지-> )
    int Dead_key = msgget((key_t)DEAD_MESSAGE_ID, IPC_CREAT|0666);
    int Dead_flag = msgget((key_t)DEAD_FLAG_ID, IPC_CREAT|0666);
    while(1){
    int returnValue = 0;
    returnValue = msgrcv(Dead_key, &DeadSignal, sizeof(DeadSignal.display_msg), 0, IPC_NOWAIT);
    if (returnValue == -1) break;
    }
    print_display_up(ground);   // 미니게임 시작되면 ground 화면 출력
    usleep(100000);

    // display.c의 displayThFunc_minigame함수를 실행하는 thread 생성
    pthread_create(&walkTh_id, NULL, &displayThFunc_minigame, NULL); 

    while(1) {
        DeadSignal.display_msg = 0;
        print_display_down(walk_state);  // 띵코리타의 위치 업데이트
        usleep(100000);

        int accel = getAcc();
        printf("%d\r\n", accel);

        // accel 센서의 기준값을 잡고, 이에 따라 띵코리타 위치 변화 
        if(accel<=-3000){ 
             walk_state = walk_right;
         }
        else if(accel>=3000){
             walk_state = walk_left;
         }
        else if(-3000 < accel < 3000){
             walk_state = walk_center;
         }
        
         // 메세지를 받아 장애물 위치 확인 
        msgrcv(Dead_key, &DeadSignal, sizeof(DeadSignal.display_msg), 0, IPC_NOWAIT); 

         // 장애물의 위치와 띵코리타가 서있는 위치가 동일하다면 장애물에 부딪힌 것으로 판정
        if(DeadSignal.display_msg == walk_state){ 
        DeadFlag.display_msg = 5; 
        msgsnd(Dead_flag, &DeadFlag, sizeof(DeadSignal.display_msg), 0);
        break; 
        }

        // 장애물에 부딪히지 않고 산책을 완료하면 체력이 하나 +1 되어 led가 켜짐 
       else if(DeadSignal.display_msg == 4) {life +=1; ledLife(life); break;} 

    }
    printf("minigame!\r\n");

    // 미니게임 성공/실패에 따라 buzzer, color led 동작
    if(DeadSignal.display_msg == 4)  
        buzzerMinigame();
    else
    {
        buzzerPlaySong(MusicScale[4]);
        color_dead();
        buzzerStopSong();
    }
}
/*====================================================================================================*/

/*======================================= 상태 업데이트 ================================================*/
int update_state(int lv_state){
   temp = temp_read();
   
   // event가 발생하면 허기, 갈증, 더러움 지수와 인게임 시간이 증가   
   hungry += 5;
   thirsty += 5;
   dirty += 10;
   hour += 1;      
 
   // 허기, 갈증, 더러움 지수는 0보다 작을 수 없음
   if(hungry < 0) hungry = 0;   
   if(thirsty < 0) thirsty = 0; 
   if(dirty < 0) dirty = 0;   

   if(hour==24)
   {
    hour = 0; //23시 이후에는 0시로 초기화
    day += 1; //24시간이 지나면 day 증가
   }     
   
   // 체력 감소 조건 확인
   if(hungry >= 100) life -= 2;
   if(thirsty >= 100) life -= 1;
   if(dirty >= 100) life -= 1;
   if(temp<20) life -= 1;  

   // 체력이 감소하면 카카오톡 메시지 발송
   if(life < pre_life) kakao();   
   pre_life = life;               // 이전값 갱신
   ledLife(life);                 // 변경된 체력에 따라 led 표시


   // AM 6:00에 레벨업 
   if(hour == 6)
    {
        color_lv_up(); // lv up시에 color led 작동
        lv_state += 1;
    }

   if(lv_state == lv_baby) // 처음 아기 상태일 때 물주기 or 밥주기 행동 한 번하면 lv up
        lv_state += 1;

   // TFT-LCD에  레벨과 온도 출력
    levelwrite(lv_state); 
    tempwrite(temp);      

    return lv_state;
}
/*====================================================================================================*/

/*================================== count thread =======================================*/
void* countTh(void* arg){

   COUNT_MSG_T Count;
   BUTTON_MSG_T Send;
   int msgID = msgget (BUTTON_MESSAGE_ID, IPC_CREAT|0666);
    int countID = msgget (COUNT_MESSAGE_ID, IPC_CREAT|0666);

    for(int i=0; i<300; i++){   
        usleep(100000);
        msgrcv(countID, &Count, sizeof(Count)-4, 0, IPC_NOWAIT);

        if(Count.count_msg == 10) pthread_exit(countTh);  // 버튼 입력이 완료되면 count thread 종료
    }
   Send.messageNum = timeout;
   msgsnd(msgID, &Send, sizeof(Send)-4,0);                // 메세지큐 사용해서 카운트 만료 정보 보냄
   pthread_exit(countTh);                                 // 카운트가 다 되면 count thread 종료
}
/*====================================================================================================*/

/*======================================= 밥주기 ======================================================*/
void feed_Func(int lv_state){ 
    // 레벨별 display 출력화면이 다르므로 CASE문으로 경우를 나눠줌
    switch(lv_state){
        case lv2: print_display_up(feed_lv2);  break;
        case lv3: print_display_up(feed_lv3);  break;
        case lv4: print_display_up(feed_lv4);  break;
        case lv5: print_display_up(feed_lv5);  break;
        case lv6: print_display_up(feed_lv6);  break;  
        case lv3_jacket: print_display_up(feed_lv3_j);  break;
        case lv4_jacket: print_display_up(feed_lv4_j);  break;
        case lv5_jacket: print_display_up(feed_lv5_j);  break;
        case lv6_jacket: print_display_up(feed_lv6_j);  break;
    }
    sleep(1);
    hungry -= 30;
    return;
}
/*====================================================================================================*/

/*======================================= 물주기 ======================================================*/
void water_Func(int lv_state){
    // 레벨별 display 출력화면이 다르므로 CASE문으로 경우를 나눠줌
    switch(lv_state){
        case lv2: print_display_up(water_lv2);  break;
        case lv3: print_display_up(water_lv3);  break;
        case lv4: print_display_up(water_lv4);  break;
        case lv5: print_display_up(water_lv5);  break;
        case lv6: print_display_up(water_lv6);  break;  
        case lv3_jacket: print_display_up(water_lv3_j);  break;
        case lv4_jacket: print_display_up(water_lv4_j);  break;
        case lv5_jacket: print_display_up(water_lv5_j);  break;
        case lv6_jacket: print_display_up(water_lv6_j);  break;
    }
    sleep(1);
    thirsty -= 30;
    return;
}
/*====================================================================================================*/

/*======================================= 청소하기 ====================================================*/
void clean_Func(int lv_state){
    // 레벨별 display 출력화면이 다르므로 CASE문으로 경우를 나눠줌
    switch(lv_state){
        case lv2: print_display_up(clean_lv2);  break;
        case lv3: print_display_up(clean_lv3);  break;
        case lv4: print_display_up(clean_lv4);  break;
        case lv5: print_display_up(clean_lv5);  break;
        case lv6: print_display_up(clean_lv6);  break;  
        case lv3_jacket: print_display_up(clean_lv3_j);  break;
        case lv4_jacket: print_display_up(clean_lv4_j);  break;
        case lv5_jacket: print_display_up(clean_lv5_j);  break;
        case lv6_jacket: print_display_up(clean_lv6_j);  break;
    }
   sleep(1);
   dirty = 0;
   return;
}
/*====================================================================================================*/











