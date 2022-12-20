#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> 
#include <sys/kd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <pthread.h>
#include "../libfbdev/libfbdev.h"
#include "../libjpeg/jpeglib.h"
#include "display.h"

int fd;
pthread_t displayTh_id1, displayTh_id2;

int displayInit(void)
{
   pthread_create(&displayTh_id1, NULL, &displayThFunc1, NULL);
    pthread_create(&displayTh_id2, NULL, &displayThFunc2, NULL);
   return 1;
}

int displayExit(void)
{
    pthread_join(displayTh_id1, NULL);
    pthread_join(displayTh_id2, NULL);
   close(fd);
   return 1;
}

/*================================= 상단이미지 ==========================================*/
void* displayThFunc1(void *arg)
{
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
    int cols = 0, rows = 0;
    char *data;
    char Displayfile[100] = "";
   //FrameBuffer init
    if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 )
   {
      printf ("FrameBuffer Init Failed\r\n");
      return 0;
   }
   
   int conFD0 = open ("/dev/tty0", O_RDWR);
   ioctl(conFD0, KDSETMODE, KD_GRAPHICS);
   close (conFD0);
   
   //Clear FB.
   fb_clear();
   

   DISPLAY_MSG_T DisplayData;
   int display_key = msgget((key_t)DISPLAY_MESSAGE_ID, IPC_CREAT|0666); //디스플레이 key 생성
   
    while(1)
    {
   int error=0;
   struct jpeg_decompress_struct cinfo;
   struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
   jpeg_create_decompress(&cinfo);
    int display_Value = 0;

    // DISPLAY_MESSAGE_ID로 메세지 받음 => 그에 따라 display 화면 변경 
    msgrcv(display_key, &DisplayData, sizeof(DisplayData.display_msg), 0, 0); 
    switch(DisplayData.display_msg)
    {
        case title: strcpy(Displayfile, "./jpg/title.jpg"); printf("title\r\n"); break;
        case lv1: strcpy(Displayfile, "./jpg/lv1.jpg"); printf("lv1\r\n"); break;
        case lv2: strcpy(Displayfile, "./jpg/lv2.jpg"); printf("lv2\r\n"); break;
        case lv3: strcpy(Displayfile, "./jpg/lv3.jpg"); printf("lv3\r\n"); break;
        case lv4: strcpy(Displayfile, "./jpg/lv4.jpg"); printf("lv4\r\n"); break;
        case lv5: strcpy(Displayfile, "./jpg/lv5.jpg"); printf("lv5\r\n"); break;
        case lv6: strcpy(Displayfile, "./jpg/lv6.jpg"); printf("lv6\r\n"); break;  
        case lv3_jacket: strcpy(Displayfile, "./jpg/lv3_jarket.jpg"); printf("lv3_jarket\r\n"); break;
        case lv4_jacket: strcpy(Displayfile, "./jpg/lv4_jarket.jpg"); printf("lv4_jarket\r\n"); break;
        case lv5_jacket: strcpy(Displayfile, "./jpg/lv5_jarket.jpg"); printf("lv5_jarket\r\n"); break;
        case lv6_jacket: strcpy(Displayfile, "./jpg/lv6_jarket.jpg"); printf("lv6_jarket\r\n"); break;
        case ground: strcpy(Displayfile, "./jpg/ground.jpg"); printf("ground\r\n"); break;
        case ground_1_1: strcpy(Displayfile, "./jpg/ground_1_1.jpg"); printf("ground_1_1\r\n"); break;
        case ground_1_2: strcpy(Displayfile, "./jpg/ground_1_2.jpg"); printf("ground_1_2\r\n"); break;
        case ground_1_3: strcpy(Displayfile, "./jpg/ground_1_3.jpg"); printf("ground_1_3\r\n"); break;
        case ground_1_4: strcpy(Displayfile, "./jpg/ground_1_4.jpg"); printf("ground_1_4\r\n"); break;
        case ground_2_1: strcpy(Displayfile, "./jpg/ground_2_1.jpg"); printf("ground_2_1\r\n"); break;
        case ground_2_2: strcpy(Displayfile, "./jpg/ground_2_2.jpg"); printf("ground_2_2\r\n"); break;
        case ground_2_3: strcpy(Displayfile, "./jpg/ground_2_3.jpg"); printf("ground_2_3\r\n"); break;
        case ground_2_4: strcpy(Displayfile, "./jpg/ground_2_4.jpg"); printf("ground_2_4\r\n"); break;
        case ground_3_1: strcpy(Displayfile, "./jpg/ground_3_1.jpg"); printf("ground_3_1\r\n"); break;
        case ground_3_2: strcpy(Displayfile, "./jpg/ground_3_2.jpg"); printf("ground_3_2\r\n"); break;
        case ground_3_3: strcpy(Displayfile, "./jpg/ground_3_3.jpg"); printf("ground_3_3\r\n"); break;
        case ground_3_4: strcpy(Displayfile, "./jpg/ground_3_4.jpg"); printf("ground_3_4\r\n"); break;
 
        case feed_lv2: strcpy(Displayfile, "./jpg/feed_lv2.jpg"); printf("feed_lv2\r\n"); break;
        case feed_lv3: strcpy(Displayfile, "./jpg/feed_lv3.jpg"); printf("feed_lv3\r\n"); break;
        case feed_lv4: strcpy(Displayfile, "./jpg/feed_lv4.jpg"); printf("feed_lv4\r\n"); break;
        case feed_lv5: strcpy(Displayfile, "./jpg/feed_lv5.jpg"); printf("feed_lv5\r\n"); break;
        case feed_lv6: strcpy(Displayfile, "./jpg/feed_lv6.jpg"); printf("feed_lv6\r\n"); break;
        case feed_lv3_j: strcpy(Displayfile, "./jpg/feed_lv3_j.jpg"); printf("feed_lv3_j\r\n"); break;
        case feed_lv4_j: strcpy(Displayfile, "./jpg/feed_lv4_j.jpg"); printf("feed_lv4_j\r\n"); break;
        case feed_lv5_j: strcpy(Displayfile, "./jpg/feed_lv5_j.jpg"); printf("feed_lv5_j\r\n"); break;
        case feed_lv6_j: strcpy(Displayfile, "./jpg/feed_lv6_j.jpg"); printf("feed_lv6_j\r\n"); break;
        case water_lv2: strcpy(Displayfile, "./jpg/water_lv2.jpg"); printf("water_lv2\r\n"); break;
        case water_lv3: strcpy(Displayfile, "./jpg/water_lv3.jpg"); printf("water_lv3\r\n"); break;
        case water_lv4: strcpy(Displayfile, "./jpg/water_lv4.jpg"); printf("water_lv4\r\n"); break;
        case water_lv5: strcpy(Displayfile, "./jpg/water_lv5.jpg"); printf("water_lv5\r\n"); break;
        case water_lv6: strcpy(Displayfile, "./jpg/water_lv6.jpg"); printf("water_lv6\r\n"); break;
        case water_lv3_j: strcpy(Displayfile, "./jpg/water_lv3_j.jpg"); printf("water_lv3_j\r\n"); break;
        case water_lv4_j: strcpy(Displayfile, "./jpg/water_lv4_j.jpg"); printf("water_lv4_j\r\n"); break;
        case water_lv5_j: strcpy(Displayfile, "./jpg/water_lv5_j.jpg"); printf("water_lv5_j\r\n"); break;
        case water_lv6_j: strcpy(Displayfile, "./jpg/water_lv6_j.jpg"); printf("water_lv6_j\r\n"); break;
        case clean_lv2: strcpy(Displayfile, "./jpg/clean_lv2.jpg"); printf("clean_lv2\r\n"); break;
        case clean_lv3: strcpy(Displayfile, "./jpg/clean_lv3.jpg"); printf("clean_lv3\r\n"); break;
        case clean_lv4: strcpy(Displayfile, "./jpg/clean_lv4.jpg"); printf("clean_lv4\r\n"); break;
        case clean_lv5: strcpy(Displayfile, "./jpg/clean_lv5.jpg"); printf("clean_lv5\r\n"); break;
        case clean_lv6: strcpy(Displayfile, "./jpg/clean_lv6.jpg"); printf("clean_lv6\r\n"); break;
        case clean_lv3_j: strcpy(Displayfile, "./jpg/clean_lv3_j.jpg"); printf("clean_lv3_j\r\n"); break;
        case clean_lv4_j: strcpy(Displayfile, "./jpg/clean_lv4_j.jpg"); printf("clean_lv4_j\r\n"); break;
        case clean_lv5_j: strcpy(Displayfile, "./jpg/clean_lv5_j.jpg"); printf("clean_lv5_j\r\n"); break;
        case clean_lv6_j: strcpy(Displayfile, "./jpg/clean_lv6_j.jpg"); printf("clean_lv6_j\r\n"); break;
        
        case inven_wear: strcpy(Displayfile, "./jpg/inven_wear.jpg"); printf("inven_wear\r\n"); break;
        case inven_unwear: strcpy(Displayfile, "./jpg/inven_unwear.jpg"); printf("inven_unwear\r\n"); break;

        case sad_ending: strcpy(Displayfile, "./jpg/sad_ending.jpg"); printf("sad_ending\r\n"); break;
        case happy_ending: strcpy(Displayfile, "./jpg/happy_ending.jpg"); printf("happy_ending\r\n"); break;
       
        default : printf("3\r\n"); continue;
    }

   FILE *fp = fopen(Displayfile, "rb");
   jpeg_stdio_src(&cinfo, fp);
   jpeg_read_header(&cinfo, TRUE); 
   //printf ("JPG %d by %d by %d, %d\n",
      //cinfo.image_width,cinfo.image_height,cinfo.num_components, cinfo.output_scanline);
   cols = cinfo.image_width;
   rows = cinfo.image_height;

   data = malloc(cols*rows*3);

   int currPoint = 0;
   jpeg_start_decompress(&cinfo);
       while(cinfo.output_scanline < cinfo.output_height) 
       {
          //printf ("CInfoScanlines:%d\r\n",cinfo.output_scanline);
          char *tempPtr=&data[currPoint];
          jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&tempPtr, 1);
          currPoint+=cols*3;
       }
   jpeg_finish_decompress(&cinfo);
   jpeg_destroy_decompress(&cinfo);
   fclose(fp);
   
   /*============= REVERSE 사용O ============*/
   // 상단 이미지는 fb_write_reverse를 사용하고, 하단이미지는 fb_write를 사용하여 사진 2개를 동시에 출력할 수 있도록
   // fb_write_reverse => 우상단부터 출력(색상반전X)
   fb_write_reverse(data, cols,rows); 
   free(data);
    }
   fb_close();

    return 0;
}
/*=====================================================================================*/

/*================================= 하단이미지 ==========================================*/
void* displayThFunc2(void *arg) 
{
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
    int cols = 0, rows = 0;
    char *data;
    char Displayfile[100] = "";
   //FrameBuffer init
    if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 )
   {
      printf ("FrameBuffer Init Failed\r\n");
      return 0;
   }
   
   int conFD0 = open ("/dev/tty0", O_RDWR);
   ioctl(conFD0, KDSETMODE, KD_GRAPHICS);
   close (conFD0);
   
   //Clear FB.
   fb_clear();
   
    DISPLAY_MSG_T DisplayData;
    int display_key = msgget((key_t)DISPLAY_MESSAGE_ID2, IPC_CREAT|0666); //디스플레이 key 생성     

    while(1)
    {
   int error=0;
   struct jpeg_decompress_struct cinfo;
   struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
   jpeg_create_decompress(&cinfo);
    int display_Value = 0;

    // DISPLAY_MESSAGE_ID2로 메세지 받음 => 그에 따라 display 화면 변경 
    msgrcv(display_key, &DisplayData, sizeof(DisplayData.display_msg), 0, 0); 
    switch(DisplayData.display_msg)
    {
        case walk_left: strcpy(Displayfile, "./jpg/walk_right.jpg"); printf("walk_right\r\n"); break;
        case walk_center: strcpy(Displayfile, "./jpg/walk_middle.jpg"); printf("walk_middle\r\n"); break;
        case walk_right: strcpy(Displayfile, "./jpg/walk_left.jpg"); printf("walk_left\r\n"); break;
        case pick_feed: strcpy(Displayfile, "./jpg/pick1.jpg"); printf("pick1\r\n"); break;
        case pick_water: strcpy(Displayfile, "./jpg/pick2.jpg"); printf("pick2\r\n"); break;
        case pick_walk: strcpy(Displayfile, "./jpg/pick3.jpg"); printf("pick3\r\n"); break;
        case pick_clean: strcpy(Displayfile, "./jpg/pick4.jpg"); printf("pick4\r\n"); break;
        case pick_inven: strcpy(Displayfile, "./jpg/pick5.jpg"); printf("pick5\r\n"); break;
        case pick_no: strcpy(Displayfile, "./jpg/pick0.jpg"); printf("pick0\r\n"); break;
        //case 10: strcpy(Displayfile, "./jpg/pick1.jpg"); printf("pick1\r\n"); break;
        default : printf("3\r\n"); continue;
    }

   FILE *fp = fopen(Displayfile, "rb");
   jpeg_stdio_src(&cinfo, fp);
   jpeg_read_header(&cinfo, TRUE); 
   //printf ("JPG %d by %d by %d, %d\n",
   //cinfo.image_width,cinfo.image_height,cinfo.num_components, cinfo.output_scanline);
   cols = cinfo.image_width;
   rows = cinfo.image_height;

   data = malloc(cols*rows*3);

   int currPoint = 0;
   jpeg_start_decompress(&cinfo);
       while(cinfo.output_scanline < cinfo.output_height) 
       {
          //printf ("CInfoScanlines:%d\r\n",cinfo.output_scanline);
          char *tempPtr=&data[currPoint];
          jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&tempPtr, 1);
          currPoint+=cols*3;
       }
   jpeg_finish_decompress(&cinfo);
   jpeg_destroy_decompress(&cinfo);
   fclose(fp);

    /*============= REVERSE 사용X ============*/
   // 상단 이미지는 fb_write_reverse를 사용하고, 하단이미지는 fb_write를 사용하여 사진 2개를 동시에 출력할 수 있도록
   // fb_write => 좌하단부터 출력(색상반전O, R과 B가 반전)
   fb_write(data, cols,rows);  
   free(data);
    }
   fb_close();

    return 0;
}
/*=======================================================================================================*/


/*============== Display thread함수(상단이미지, 하단이미지)에 각각 메세지를 보내주는 함수 ====================*/
// tting.c에서 함수 사용하여 display thread 함수에 메세지 보냄
int print_display_up(int i){
    DISPLAY_MSG_T DisplayData;
    int display_key = msgget((key_t)DISPLAY_MESSAGE_ID, IPC_CREAT|0666); //디스플레이 key 생성

    DisplayData.display_msg = i; 
    DisplayData.messageType = 1;
    msgsnd(display_key, &DisplayData, sizeof(DisplayData.display_msg), 0); // 메세지 보냄
}

int print_display_down(int i){
    DISPLAY_MSG_T DisplayData;
    int display_key = msgget((key_t)DISPLAY_MESSAGE_ID2, IPC_CREAT|0666); 

    DisplayData.display_msg = i;
    DisplayData.messageType = 1;
    msgsnd(display_key, &DisplayData, sizeof(DisplayData.display_msg), 0);
}
/*=======================================================================================================*/

/*================================= 미니게임 ground의 thread ======================================*/
void* displayThFunc_minigame(void *arg)
{  
    DISPLAY_MSG_T DeadSignal;   // 장애물이 어느 위치에 있는지 알리는 메세지 구조체(thread가 보내는 메세지)
    DISPLAY_MSG_T DeadFlag;     // 띵코리타와 장애물이 닿았을 시 알리는 메세지 구조체(thread가 받는 메세지)

    int Dead_key = msgget((key_t)DEAD_MESSAGE_ID, IPC_CREAT|0666);  // 장애물이 어느 위치에 있는지 알리는 메세지
    int Dead_flag = msgget((key_t)DEAD_FLAG_ID, IPC_CREAT|0666);    // 띵코리타와 장애물이 닿았을 시 알리는 메세지 

    // ground 화면을 출력하고, 장애물 위치를 알리는 메세지를 보내는 for문
    for(int i = 19; i<31; i++)
    {
        sleep(1);
        print_display_up(i);

        if(i==ground_1_4){
            DeadSignal.display_msg = 1; // 장애물이 1번 레인에 있음
            DeadSignal.messageType = 1;
            msgsnd(Dead_key, &DeadSignal, sizeof(DeadSignal.display_msg), 0);
        }
        else if(i==ground_2_4){
            DeadSignal.display_msg = 2; // 장애물이 2번 레인에 있음
            DeadSignal.messageType = 1;
            msgsnd(Dead_key, &DeadSignal, sizeof(DeadSignal.display_msg), 0);
        }
        else if(i==ground_3_4){
            DeadSignal.display_msg = 3; // 장애물이 3번 레인에 있음
            DeadSignal.messageType = 1;
            msgsnd(Dead_key, &DeadSignal, sizeof(DeadSignal.display_msg), 0);
        }

        // 띵코리타가 장애물과 부딪혔다면 메세지로 DeadFlag.display_msg == 5를 받음
        msgrcv(Dead_flag, &DeadFlag, sizeof(DeadFlag.display_msg), 0, IPC_NOWAIT);     
        if(DeadFlag.display_msg == 5) break; // 미니게임 실패시 DeadFlag.display_msg == 5
    }
    sleep(1);
    print_display_up(ground);
    // tting.c의 minigame_Func에 미니게임 성공 signal 메세지 보냄    
    DeadSignal.display_msg = 4; // 미니게임 성공시 DeadSignal.display_msg == 4
    DeadSignal.messageType = 1;
    msgsnd(Dead_key, &DeadSignal, sizeof(DeadSignal.display_msg), 0);
}


