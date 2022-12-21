#ifndef __TEXTLCD_H
#define __TEXTLCD_H

#define TXTLCD_ID 1000

// TextLCD 메세지 구조체
typedef struct
{
   long int messageNum;
   int lv_state; // Level 반영 
} TXTLCD_LEVEL_MSG;

int levelwrite(int lv_state); // TextLCD 첫째줄 Level 출력
int tempwrite(float temp);    // TextLCD 둘째줄 온도 출력
int lcdtextwrite(const char *str1, const char *str2, int lineFlag);
int textlcdInit(void);
int textlcdExit(void);
void* textlcdTh(void *arg);

#endif
