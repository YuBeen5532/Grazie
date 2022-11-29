#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include "textlcddrv.h" 
#include "textlcd.h"

int fd;

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
    write(fd,&stlcd,sizeof(stTextLCD));  
}

int textlcdInit(void){
    fd = open("/dev/peritextlcd",O_RDWR); // open driver
    if ( fd < 0 ) { 
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
    close(fd);
    return 0;
}