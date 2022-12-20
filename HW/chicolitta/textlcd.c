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
// 1.레벨 2.온도


int levelwrite(int lv_state){ // level 첫째줄
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

int tempwrite(float temp){ // temp 둘째줄  
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


int lcdtextwrite(const char *str1, const char *str2, int lineFlag){

   unsigned int linenum = 0;
   stTextLCD stlcd; // stTextLCD 구조체를 가지고 드라이버와 인터페이스
   
   int len1, len2;
   memset(&stlcd,0,sizeof(stTextLCD)); // 구조체 초기화
   linenum = lineFlag;
   printf("linenum :%d\n", linenum);

   //첫번째줄 또는 두번째줄 중 한 줄 선택해서 쓰기
   if (linenum == 1){
      if (str2==NULL) { // 1번째 줄 쓰기
         stlcd.cmdData = CMD_DATA_WRITE_LINE_1; // 0
         len1 = strlen(str1);      // 문자열길이
         if (len1 > COLUMN_NUM)    //한 줄에 들어갈 수 있는 문자길이 16
         memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, COLUMN_NUM);//몇번째 라인에 쓸지 메모리를 복사?
         // 복사받을, 복사할, 복사할데이터길이
         else
         memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, len1);
      }
      else if (str1==NULL){ // 2번째 줄 쓰기
         stlcd.cmdData = CMD_DATA_WRITE_LINE_2; // 1
         len2 = strlen(str2);      // 문자열길이
         if (len2 > COLUMN_NUM)    // 한 줄에 들어갈 수 있는 문자길이 16
         memcpy(stlcd.TextData[stlcd.cmdData - 1], str2, COLUMN_NUM);//몇번째 라인에 쓸지 메모리를 복사?
         // 복사받을, 복사할, 복사할데이터길이
         //
         else
         memcpy(stlcd.TextData[stlcd.cmdData - 1], str2, len2);
      }
   }

   // 첫번째줄, 두번째줄 동시에 쓰기 또한 가능
   else if ( linenum == 2){
      stlcd.cmdData = CMD_DATA_WRITE_BOTH_LINE;
      len1 = strlen(str1);
      len2 = strlen(str2);
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

int textlcdInit(void){
   fd_txt = open("/dev/peritextlcd",O_RDWR); // open driver
   if ( fd_txt < 0 ) {
      perror("driver (//dev//peritextlcd) open error.\n");
   }
   return 1;
}

int textlcdExit(void){
   //stTextLCD stlcd;
   //memset(&stlcd,0,sizeof(stTextLCD)); // 구조체 초기화
   //write(fd,&stlcd,sizeof(stTextLCD));
   //char[20]
   //lcdtextwrite(NULL,text2,);
   close(fd_txt);
   return 0;
}
