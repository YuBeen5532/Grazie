#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#define DISPLAY_MESSAGE_ID 100
#define DISPLAY_MESSAGE_ID2 150
#define DEAD_MESSAGE_ID 120
#define DEAD_FLAG_ID 110

#define title 1
#define lv1 2 // 새싹
#define lv2 3 // 눈감은
#define lv3 4 // 새싹1
#define lv4 5 // 새싹3
#define lv5 6 // 새싹5
#define lv6 7 // 새싹7
#define lv3_jacket 14
#define lv4_jacket 15
#define lv5_jacket 16
#define lv6_jacket 17
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

#define feed_lv2 33 // 눈감은 상태 밥먹기 => feed_state = lv_state+30
#define feed_lv3 34 // 새싹1 밥먹기 
#define feed_lv4 35 // 새싹3 밥먹기 
#define feed_lv5 36 // 새싹5 밥먹기 
#define feed_lv6 37 // 새싹7 밥먹기 
#define feed_lv3_j 44 // 새싹1 밥먹기
#define feed_lv4_j 45 // 새싹3 밥먹기
#define feed_lv5_j 46 // 새싹5 밥먹기
#define feed_lv6_j 47// 새싹7 밥먹기

#define water_lv2 53 // 눈감은 상태 밥먹기 => feed_state = lv_state+50
#define water_lv3 54 // 새싹1 물먹기
#define water_lv4 55 // 새싹3 밥먹기
#define water_lv5 56 // 새싹5 밥먹기
#define water_lv6 57 // 새싹7 밥먹기
#define water_lv3_j 64 // 새싹1 옷입고 밥먹기
#define water_lv4_j 65 // 새싹3 옷입고 밥먹기
#define water_lv5_j 66 // 새싹5 옷입고 밥먹기
#define water_lv6_j 67// 새싹7 옷입고 밥먹기

#define clean_lv2 73 // 눈감은 상태 청소하기 => feed_state = lv_state+70
#define clean_lv3 74
#define clean_lv4 75
#define clean_lv5 76
#define clean_lv6 77
#define clean_lv3_j 84
#define clean_lv4_j 85
#define clean_lv5_j 86
#define clean_lv6_j 87

#define inven_wear 90
#define inven_unwear 91

#define sad_ending 100
#define happy_ending 200
// 물주기 밥먹기 청소하기 화면도 레벨과 옷별로 다시 다 해줘야함

//pick_state
#define pick_feed 4
#define pick_water 5
#define pick_walk 6
#define pick_clean 7
#define pick_inven 8
#define pick_no 9

//walk_state
#define walk_left 1
#define walk_center 2
#define walk_right 3

//lv_state
#define lv_clova 2
#define lv_baby 3
#define lv_kid1 4
#define lv_kid2 5
#define lv_kid3 7
#define lv_kid4 8

//pick_button
#define pick_btn_wear 2
#define pick_btn_left 3
#define pick_btn_right 4
#define pick_btn_exit 5
#define pick_btn_lv 6


typedef struct
{
long int messageType;
int display_msg; //(타이틀화면1, 성장화면2, 밥주기3, 물주기4, 청소하기5)
} DISPLAY_MSG_T;

int displayInit(void);
int displayExit(void);
void* displayThFunc1(void *arg);
void* displayThFunc2(void *arg);
int print_display_up(int i);
int print_display_down(int i);
void* displayThFunc_minigame(void *arg);
#endif

