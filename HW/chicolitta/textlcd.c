#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include "textlcddrv.h"
#include "textlcd.h"

int fd_txt;

/*======================== TextLCD 첫째줄 Level 출력 ===========================*/
int levelwrite(int lv_state)
{ 
   lv_state=lv_state % 10;
   stTextLCD stlcd;
   char *level;
   switch(lv_state){
        case 3: level="Level 1"; break;
        case 4: level="Level 2"; break;
        case 5: level="Level 3"; break;
        case 6: level="Level 4"; break;
        case 7: level="Level 5"; break;
        default: level="TTINGKORITA"; break;
    }
    memset(&stlcd,0,sizeof(stTextLCD));
    stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
    stlcd.cmd = CMD_WRITE_STRING;   
    memcpy(stlcd.TextData[0], level, COLUMN_NUM);
    write(fd_txt,&stlcd,sizeof(stTextLCD));
    printf("%s\r\n",level);
    return 1;
}
/*===================================================================================*/

/*======================== TextLCD 둘째줄 온도 출력 ==============================*/
int tempwrite(float temp)
{   
    char temperature[100];
    sprintf(temperature,"temp : %f", temp);
    printf("%s\r\n",temperature);
    stTextLCD stlcd;
    memset(&stlcd,0,sizeof(stTextLCD));
    stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
    stlcd.cmd = CMD_WRITE_STRING;
    memcpy(stlcd.TextData[1],temperature, COLUMN_NUM);
    write(fd_txt,&stlcd,sizeof(stTextLCD));
    return 1;
}
/*===================================================================================*/

/*======================== TextLCD Write =================================*/
int lcdtextwrite(const char *str1, const char *str2, int lineFlag)
{
    unsigned int linenum = 0;
    stTextLCD stlcd; // stTextLCD 구조체를 가지고 드라이버와 인터페이스
   
    int len1, len2;
    memset(&stlcd,0,sizeof(stTextLCD)); // 구조체 초기화
    linenum = lineFlag;
    printf("linenum :%d\n", linenum);

     // 첫번째 줄 또는 두번째 줄 중 한 줄 선택해서 쓰기
    if (linenum == 1){
         // 첫번째 줄 쓰기
        if (str2==NULL) {                          
            stlcd.cmdData = CMD_DATA_WRITE_LINE_1; 
            len1 = strlen(str1);      // 문자열 길이
              // 문자열 길이 > 16: 16만큼만 복사 / 문자열 길이 < 16: 문자열 길이만큼 복사
            if (len1 > COLUMN_NUM)    
                memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, COLUMN_NUM); // 몇번째 라인에 쓸지 메모리 복사
            else
                memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, len1);
         }
         // 두번째 줄 쓰기
        else if (str1==NULL){                      
            stlcd.cmdData = CMD_DATA_WRITE_LINE_2; 
            len2 = strlen(str2);      
            if (len2 > COLUMN_NUM)    
                memcpy(stlcd.TextData[stlcd.cmdData - 1], str2, COLUMN_NUM); 
            else
                memcpy(stlcd.TextData[stlcd.cmdData - 1], str2, len2);
         }
     }

     // 첫번째줄, 두번째줄 모두 쓰기
    else if ( linenum == 2){
        stlcd.cmdData = CMD_DATA_WRITE_BOTH_LINE;
        len1 = strlen(str1);
        len2 = strlen(str2);
         // 문자열 길이 > 16: 16만큼만 복사 / 문자열 길이 < 16: 문자열 길이만큼 복사
        if (len1 > COLUMN_NUM)
            memcpy(stlcd.TextData[0], str1, COLUMN_NUM);
        else
            memcpy(stlcd.TextData[0], str1, len1);

        if (len2 > COLUMN_NUM)
            memcpy(stlcd.TextData[1], str2, COLUMN_NUM);
        else
            memcpy(stlcd.TextData[1], str2, len2);
   }
    else {
        printf("linenum : %d wrong . range (1 ~ 2)\n", linenum);
        return 1; }
   
    stlcd.cmd = CMD_WRITE_STRING;
    write(fd_txt,&stlcd,sizeof(stTextLCD));
}
/*===================================================================================*/

/*======================== TextLCD INIT ==================================*/
int textlcdInit(void){
   fd_txt = open("/dev/peritextlcd",O_RDWR); // driver open 
   if ( fd_txt < 0 ) {
      perror("driver (//dev//peritextlcd) open error.\n");
   }
   return 1;
}
/*===================================================================================*/

/*======================== TextLCD EXIT ==================================*/
int textlcdExit(void)
{
   close(fd_txt); //  driver close
   return 0;
}
/*===================================================================================*/
