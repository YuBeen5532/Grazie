#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#define DISPLAY_MESSAGE_ID 100
#define DISPLAY_MESSAGE_ID2 150
#define DEAD_MESSAGE_ID 120
#define DEAD_FLAG_ID 110

// Dislay 메세지를 위한 구조체
typedef struct
{
long int messageType;
int display_msg; 
} DISPLAY_MSG_T;

#define title 1          // title 화면
#define lv1 2            // 땅에 난 새싹
#define lv2 3            // 아기 띵코리타
#define lv3 4            // 새싹1개 띵코리타
#define lv4 5            // 새싹3개 띵코리타
#define lv5 6            // 새싹5개 띵코리타
#define lv6 7            // 새싹7개 띵코리타

#define lv3_jacket 14    // 새싹1개 띵코리타 + 옷입음
#define lv4_jacket 15    // 새싹3개 띵코리타 + 옷입음
#define lv5_jacket 16    // 새싹5개 띵코리타 + 옷입음
#define lv6_jacket 17    // 새싹7개 띵코리타 + 옷입음

// 밥먹는 이미지 (feed_lv = lv+30) //
#define feed_lv2 33 
#define feed_lv3 34 
#define feed_lv4 35 
#define feed_lv5 36 
#define feed_lv6 37 
// 밥먹는 이미지 + 옷착용 (feed_lv = lv+40) //
#define feed_lv3_j 44 
#define feed_lv4_j 45 
#define feed_lv5_j 46 
#define feed_lv6_j 47

// 물먹는 이미지 (water_lv = lv+50) //
#define water_lv2 53 
#define water_lv3 54 
#define water_lv4 55 
#define water_lv5 56 
#define water_lv6 57 
// 물먹는 이미지 + 옷착용 (water_lv = lv+60) //
#define water_lv3_j 64 
#define water_lv4_j 65 
#define water_lv5_j 66 
#define water_lv6_j 67

// 청소하는 이미지 (clean_lv = lv+70) //
#define clean_lv2 73
#define clean_lv3 74
#define clean_lv4 75
#define clean_lv5 76
#define clean_lv6 77
// 청소하는 이미지 + 옷착용 (clean_lv = lv+80) //
#define clean_lv3_j 84
#define clean_lv4_j 85
#define clean_lv5_j 86
#define clean_lv6_j 87

// 인벤토리 안에서 착용/해제 화면 //
#define inven_wear 90
#define inven_unwear 91

// 엔딩화면 //
#define sad_ending 100
#define happy_ending 200

// ground 이미지 //
#define ground 18
#define ground_1_1 19
#define ground_1_2 20
#define ground_1_3 21
#define ground_1_4 22
#define ground_2_1 23
#define ground_2_2 24 
#define ground_2_3 25
#define ground_2_4 26
#define ground_3_1 27
#define ground_3_2 28
#define ground_3_3 29
#define ground_3_4 30

// 산책시 띵코리타 위치 //
#define walk_left 1
#define walk_center 2
#define walk_right 3

// 현재 레벨 //
#define lv_clova 2
#define lv_baby 3
#define lv_kid1 4
#define lv_kid2 5
#define lv_kid3 7
#define lv_kid4 8

// 어떤 행동 선택했는지 //
#define pick_feed 4
#define pick_water 5
#define pick_walk 6
#define pick_clean 7
#define pick_inven 8
#define pick_no 9

// 어떤 버튼 눌렀는지 //
#define pick_btn_left 3
#define pick_btn_right 4
#define pick_btn_exit 5
#define pick_btn_lv 6

int displayInit(void);
int displayExit(void);
void* displayThFunc1(void *arg);
void* displayThFunc2(void *arg);
int print_display_up(int i);
int print_display_down(int i);
void* displayThFunc_minigame(void *arg);
#endif

