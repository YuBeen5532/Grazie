#ifndef __TEXTLCD_H
#define __TEXTLCD_H
int textlcdInit(void);
int textlcdExit(void);
int lcdtextwrite(const char *str1, const char *str2, int lineFlag);

#endif