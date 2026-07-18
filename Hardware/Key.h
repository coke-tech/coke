#ifndef __KEY_H
#define __KEY_H
void Key_Init(void);
void Key_scan(void);
uint8_t Key_GetNum(void);
uint8_t Key_GetState(void);
void Key_Tick(void);
extern uint8_t Rse1_flag, Rse2_flag, Rse3_flag,Rse5_flag,Rse7_flag,Rse8_flag;
extern uint8_t lcd_mode;
extern uint8_t time_cg;
extern uint8_t Key_Num;
extern uint8_t dino_game_active;
#endif
