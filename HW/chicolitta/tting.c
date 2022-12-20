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

pthread_t walkTh_id, countTh_id;

#define MAX_SCALE_STEP 8 

const int MusicScale[8] =
{
    262, /*do*/ 294,330,349,392,440,494, /* si */ 523
};

float temp;
int life=8;
int pre_life=8;
int hungry=0;
int thirsty=0;
int dirty=0;
int hour=6;
int day = 1;
int time_trig=0;


void* countTh(void* arg){
   COUNT_MSG_T Count;
   BUTTON_MSG_T Send;
   int msgID = msgget (BUTTON_MESSAGE_ID, IPC_CREAT|0666);
    int countID = msgget (COUNT_MESSAGE_ID, IPC_CREAT|0666);

    for(int i=0; i<300; i++){   
        usleep(100000);
        msgrcv(countID, &Count, sizeof(Count)-4, 0, IPC_NOWAIT);
        if(Count.count_msg == 10) pthread_exit(countTh);
    }
   Send.messageNum = timeout;
   msgsnd(msgID, &Send, sizeof(Send)-4,0);                // 메세지큐 사용해서 카운트 만료 정보 보냄
   pthread_exit(countTh);
}


void feed_Func(int lv_state){ // 레벨별로 다 다르니까 레벨변수하나 지정하고, 케이스문 나눠서 함수 선정하기.
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
   //evnet 발생 시 상태변경
   hungry -= 30;
   return;
}


void water_Func(int lv_state){
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
   //evnet 발생 시 상태변경
   thirsty -= 30;
   return;
}


void clean_Func(int lv_state){
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
   //evnet 발생 시 상태변경
   dirty = 0;
   return;
}


int inven_Func(int lv_state){ 
   int inven_state=inven_wear;   
   print_display_up(inven_state);
   usleep(100000);
   int select=0;   
         
   while(1){ // OK버튼이 눌리면 while문 탈출
      select=button_rcv();
      if(select==3){ // left버튼 
         if(inven_state==inven_wear)
            inven_state=inven_unwear;
         else 
            inven_state=inven_wear;
         print_display_up(inven_state);
      }
      else if(select==4){ // right버튼   
         if(inven_state==inven_unwear) 
            inven_state=inven_wear;
         else
            inven_state=inven_unwear;
         print_display_up(inven_state);
      }
      else if(select == 5) break;
   }
   if(inven_state==inven_wear && lv_state<10)
      lv_state=lv_state+10;   
   else if(inven_state==inven_unwear && lv_state>10)
      lv_state = lv_state-10;   
   
   print_display_up(lv_state);
   usleep(100000);
      
   return lv_state;
}


void minigame_Func(){

	int walk_state=walk_center;  // 산책시 기본값: 중앙 위치
    DISPLAY_MSG_T DeadSignal;
    DISPLAY_MSG_T DeadFlag;
    int Dead_key = msgget((key_t)DEAD_MESSAGE_ID, IPC_CREAT|0666);
    int Dead_flag = msgget((key_t)DEAD_FLAG_ID, IPC_CREAT|0666);
    while(1){
    int returnValue = 0;
    returnValue = msgrcv(Dead_key, &DeadSignal, sizeof(DeadSignal.display_msg), 0, IPC_NOWAIT);
    if (returnValue == -1) break;
    }
    print_display_up(ground); // 산책 ground
    usleep(100000);
    pthread_create(&walkTh_id, NULL, &displayThFunc_minigame, NULL);
    while(1) {
        DeadSignal.display_msg = 0;
        print_display_down(walk_state); // 띵코리타의 위치 업데이트
        usleep(100000);

        int accel = getAcc();
        printf("%d\r\n", accel);
        if(accel<=-3000){ // case문으로 바꾸기
             walk_state = walk_right;
          }
         else if(accel>=3000){
             walk_state = walk_left;
          }
         else if(-3000 < accel < 3000){
             walk_state = walk_center;
          }
         
       msgrcv(Dead_key, &DeadSignal, sizeof(DeadSignal.display_msg), 0, IPC_NOWAIT); //Dead Signal 확인
       if(DeadSignal.display_msg == walk_state){
        DeadFlag.display_msg = 5;
        msgsnd(Dead_flag, &DeadFlag, sizeof(DeadSignal.display_msg), 0);
        break; //부딪혔다면 while문 탈출(돌부리 레인 == 현재 레인)
        }
       else if(DeadSignal.display_msg == 4) {life +=1; ledLife(life); break;} //미니게임 종료
    }
    printf("minigame!\r\n");
    if(DeadSignal.display_msg == 4)
    buzzerMinigame();

    else{
    buzzerPlaySong(MusicScale[4]);
    color_dead();
    buzzerStopSong();
    }
    return;
}

int update_state(int lv_state){
   temp = temp_read();
   
   //카운트가 들어오면      
   hungry += 5;
   thirsty += 5;
   dirty += 10;
   hour += 1;      
 
   if(hungry < 0) hungry = 0; //0보다 작아지면 0으로 초기화
   if(thirsty < 0) thirsty = 0; //0보다 작아지면 0으로 초기화
   if(dirty < 0) dirty = 0; //0보다 작아지면 0으로 초기화
   if(hour==24)
   {
    hour = 0; //23시 이후에는 0시로 초기화
    day += 1; //24시간이 지나면 day 증가
   }
     
   
   // 체력 감소 조건 확인.
   if(hungry >= 100) life -= 2;
   if(thirsty >= 100) life -= 1;
   if(dirty >= 100) life -= 1;
   if(temp<20) life -= 1;  
   // tting main문에 체력==0이면 게임종료를 만들어줘야한다.  
   if(life < pre_life) kakao();
    // 피가 깎이면 카카오톡 메시지 발송
    pre_life = life; //이전값 갱신

   ledLife(life);   // 변경된 체력에 따라 led 표시

    if(hour == 6){
   color_lv_up();
    //버저 울려준다.
    //
    if(lv_state != lv6 && lv_state != lv6_jacket)
        lv_state += 1;
    }
    if (lv_state == lv_baby)
    lv_state += 1;

    levelwrite(lv_state);
    tempwrite(temp);

    return lv_state;


}


int main(void)
{
    accInit();              //Accel
    FindBuzzerSysPath();    //Buzzer
    pwmLedInit();           //colorLED
    displayInit();          //TFT-LCD
    buttonInit();           //Button
    ledInit();              //LED
    fndInit();              //FND
    textlcdInit();    //TXT-LCD

    //온도센서는 init 없음
    
/////////////////////////////////////////////////////////////////////
while(1){
    int pick_state=pick_feed;  // display에 pick_state를 보냄 
    // DisplayData.display_msg == 4~8
    // pick1(4): 밥주기 pick2(5): 물주기 pick3(6): 산책하기 pick4(7): 청소하기 pick5(8): 인벤토리

    int lv_state=lv_baby;    // 기본값: 새싹
    int pick_button;
    print_display_up(title);  // title화면
    usleep(100000);
    fnd_hour(hour , day);
    ledLife(life);
    levelwrite(lv_state);
    tempwrite(temp);

    while(1)
    {
        pick_button = button_rcv(); // 버튼의 입력받아 
        if(pick_button == pick_btn_exit) break;        // 홈key 확인 => break. while문 탈출.
    }
    
//////////////////////////////////////////////
        
    print_display_up(lv_clova);
    print_display_down(pick_no);
    sleep(1);
    
//////////////////////////////////////////////
    while(1){
    printf("hungry : %d\n\r",hungry);
    printf("thirsty : %d\n\r",thirsty);
    printf("dirty : %d\n\r",dirty);
    printf("life : %d\n\r",life);
    printf("day : %d\n\r",day);
    printf("hour : %d\n\r",hour);
    printf("temp : %f\n\r",temp);
    pthread_create(&countTh_id, NULL, &countTh, NULL); // 카운트 thread 켜주고
    while(1)
    {
        
        print_display_up(lv_state);      // 현재 level의 띵코리타 모습 
        print_display_down(pick_state);  // 현재 display상에서의 선택        
        usleep(100000);              // 화면이 깨지지 않도록 delay
        fnd_hour(hour , day);

        pick_button = 0;
        pick_button = button_rcv(); // 어떤 행동할지 고르는 버튼 입력

        if(pick_button == pick_btn_left) // pick_state == 4~8, pick_button ==3 => 왼쪽으로 
         {
            if(pick_state>=5) pick_state -= 1; // pick_state==5 
            else if(pick_state==4) pick_state = 8;
         }
        if(pick_button == pick_btn_right) // pick_button ==4 => 오른쪽으로 
         {
            if(pick_state<=7) pick_state += 1;
            else if(pick_state==8) pick_state = 4;
         }
        if(pick_button == pick_btn_lv) // pick_button ==6 => Lv Up 
         {
            if((lv_state>=2 && lv_state<=6) || (lv_state>=14 && lv_state<=16)) lv_state += 1;
         } // 레벨업 확인용 & 자켓입었을 경우

        if(pick_button == pick_btn_exit){
            if(lv_state == lv_baby && (pick_state == 6 || pick_state == 7 || pick_state == 8 )) continue; 
            else break;} // 선택종료
        if(pick_button == timeout && time_trig == 0) {time_trig =1; break;}
        if(pick_button == timeout) {time_trig = 0; pthread_create(&countTh_id, NULL, &countTh, NULL); }
    }

/////////////////////////////////////////////////////////
    COUNT_MSG_T Count;
    int countID = msgget (COUNT_MESSAGE_ID, IPC_CREAT|0666);
    Count.count_msg = 10;
    msgsnd(countID, &Count, sizeof(Count)-4, 0);
    if(pick_button != timeout){
    switch(pick_state)                          //행동하기
    {
        case 4: feed_Func(lv_state);  break;     // 밥주기
        case 5: water_Func(lv_state); break;     // 물주기
        case 6: minigame_Func(lv_state); break;  // 산책하기
        case 7: clean_Func(lv_state); break;     // 청소하기
        case 8: lv_state = inven_Func(lv_state); break;     // 인벤토리 (lv 낮을때 못들어가게 해야함)
    }
    }
////////////////////////////////////////////////////////
    lv_state = update_state(lv_state);          //상태변경


    if((lv_state % 10 == 7) && (hour == 6)) { print_display_up(happy_ending); break;}
    if(life<=0){print_display_up(sad_ending); break;}
    }

    while(1)
    {
        pick_button = button_rcv(); // 버튼의 입력받아 
        if(pick_button == pick_btn_exit) break;        // 홈key 확인 => break. while문 탈출.
    }
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








