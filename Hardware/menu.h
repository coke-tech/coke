#ifndef __MENU_H
#define __MENU_H

void comInit(void);
void Show_Clock_UI(void);
void Show_SettingPage_UI(void);
void LCD_show(void);
void LCD_show_UI0(void);
void LCD_show_UI1(void);
void LCD_show_UI2(void);
void LCD_show_UI3(void);
void LCD_show_UI4(void);
void LCD_show_UI5(void);
void LCD_show_UI6(void);
void LCD_show_UI7(void);
void Menu_Animation(void);
void MPU6050_Calculation(void);
void Show_MPU6050_UI(void);
void Show_LED_UI(void);
void Show_StopWatch_UI(void);
void StopWatch_Tick(void);

extern uint8_t pre_selection;
extern uint8_t target_selection;
extern uint8_t x_pre;
extern uint8_t Speed;
extern uint8_t move_flag;
extern uint8_t hour,min,sec;
extern uint8_t start_timing_flag;

#endif