#ifndef __TEXTLCD_H
#define __TEXTLCD_H
int textlcdInit(void);
int textlcdExit(void);
int lcdtextwrite(const char *str1, const char *str2, int lineFlag);
void* textlcdTh(void *arg);
int levelwrite(int lv_state);
int tempwrite(float temp);

typedef struct
{
   long int messageNum;
   int lv_state;
}TXTLCD_LEVEL_MSG ;

#define TXTLCD_ID 1000

#endif
