#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "OLED_Data.h"
#include "MyRTC.h"
#include "Key.h"
#include "Timer.h"
#include "Setime.h"
#include "LED.h"
#include "MPU6050.h"

void comInit(void)
{
	MyRTC_Init();
	Key_Init();
	Timer_Init();
	LED_Init();
	MPU6050_Init();
}

void Show_Clock_UI(void)
{
	MyRTC_ReadTime();
	OLED_Printf(0,0,OLED_6X8,"%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
	OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
	OLED_ShowString(0,48,"菜单",OLED_8X16);
	OLED_ShowString(96,48,"设置",OLED_8X16);
}
void Show_SettingPage_UI(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(0,16,"日期时间设置",OLED_8X16);
}

uint8_t pre_selection;
uint8_t target_selection;
uint8_t x_pre=48;
uint8_t Speed=4;
uint8_t move_flag;

void Menu_Animation(void)
{
	OLED_Clear();
	OLED_ShowImage(42,10,44,44,Frame);

	if(pre_selection<target_selection)
	{
		x_pre-=Speed;
		if(x_pre==0)
		{
			pre_selection++;
			move_flag=0;
			x_pre=48;
		}
	}

	if(pre_selection>target_selection)
	{
		x_pre+=Speed;
		if(x_pre==96)
		{
			pre_selection--;
			move_flag=0;
			x_pre=48;
		}
	}

	if(pre_selection>=1)
	{
		OLED_ShowImage(x_pre-48,16,32,32,Menu_Graph[pre_selection-1]);
	}

	if(pre_selection>=2)
	{
		OLED_ShowImage(x_pre-96,16,32,32,Menu_Graph[pre_selection-2]);
	}

	OLED_ShowImage(x_pre,16,32,32,Menu_Graph[pre_selection]);
	OLED_ShowImage(x_pre+48,16,32,32,Menu_Graph[pre_selection+1]);
	OLED_ShowImage(x_pre+96,16,32,32,Menu_Graph[pre_selection+2]);

	OLED_Update();
}

void LCD_show_UI0(void)
{
	OLED_Clear();
	Show_Clock_UI();

	if(Rse1_flag == 0)
	{
		OLED_ReverseArea(0,48,32,16);
	}
	else
	{
		OLED_ReverseArea(96,48,32,16);
	}

	
	OLED_Update();
}

void LCD_show_UI1(void)
{
	OLED_Clear();
	Show_SettingPage_UI();

	if(Rse2_flag == 0)
	{
		OLED_ReverseArea(0,0,16,16);
	}
	else
	{
		OLED_ReverseArea(0,16,96,16);
	}

	OLED_Update();
}

void LCD_show_UI2(void)
{
	OLED_Clear();
	Show_SetTime_FirstUI();

	switch(Rse3_flag)
	{
		case 0:
			OLED_ReverseArea(0,0,16,16);
			break;
		case 1:
			if(time_cg == 0)
			{
				OLED_ReverseArea(0,16,16,16);
			}
			else
			{
				OLED_ReverseArea(16,16,80,16);
			}
			break;
		case 2:
			if(time_cg == 0)
			{
				OLED_ReverseArea(0,32,16,16);
			}
			else
			{
				OLED_ReverseArea(16,32,80,16);
			}
			break;
		case 3:
			if(time_cg == 0)
			{
				OLED_ReverseArea(0,48,16,16);
			}
			else
			{
				OLED_ReverseArea(16,48,80,16);
			}
			break;
	}

	OLED_Update();
}

void LCD_show_UI3(void)
{
	OLED_Clear();
	Show_SetTime_SecondUI();

	switch(Rse3_flag)
	{
		case 0:
			if(time_cg == 0)
			{
				OLED_ReverseArea(0,0,16,16);
			}
			else
			{
				OLED_ReverseArea(16,0,80,16);
			}
			break;
		case 1:
			if(time_cg == 0)
			{
				OLED_ReverseArea(0,16,16,16);
			}
			else
			{
				OLED_ReverseArea(16,16,80,16);
			}
			break;
		case 2:
			if(time_cg == 0)
			{
				OLED_ReverseArea(0,32,16,16);
			}
			else
			{
				OLED_ReverseArea(16,32,80,16);
			}
			break;
	}

	OLED_Update();
}

void LCD_show_UI4(void)
{
	Menu_Animation();
}

void Show_LED_UI(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(20,20,"OFF",OLED_12X24);
	OLED_ShowString(72,20,"ON",OLED_12X24);
}

void LCD_show_UI5(void)
{
	OLED_Clear();
	Show_LED_UI();
	switch(Rse5_flag)
	{
		case 0:
			OLED_ReverseArea(0,0,16,16);
		break;
		case 1:
			OLED_ReverseArea(20,20,36,24);
		break;
		case 2:
			OLED_ReverseArea(72,20,24,24);
		break;
	}
	OLED_Update();
}

int16_t ax,ay,az,gx,gy,gz;
float roll_g,pitch_g,yaw_g;
float roll_a,pitch_a;
float Roll,Pitch,Yaw;
float a=0.9;
float Delta_t=0.005;
double pi=3.1415927;
extern uint8_t MPU_S;

void MPU6050_Calculation(void)
{
	if(MPU_S>5)
	{
		MPU6050_GetData(&ax,&ay,&az,&gx,&gy,&gz);

		roll_g=Roll+(float)gx*Delta_t;
		pitch_g=Pitch+(float)gy*Delta_t;
		yaw_g=Yaw+(float)gz*Delta_t;

		pitch_a=atan2((-1)*ax,az)*180/pi;
		roll_a=atan2(ay,az)*180/pi;

		Roll=a*roll_g+(1-a)*roll_a;
		Pitch=a*pitch_g+(1-a)*pitch_a;
		Yaw=a*yaw_g;

		MPU_S=0;
	}
}

void Show_MPU6050_UI(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(0,16,OLED_8X16,"Roll: %.2f",Roll);
	OLED_Printf(0,32,OLED_8X16,"Pitch:%.2f",Pitch);
	OLED_Printf(0,48,OLED_8X16,"Yaw:  %.2f",Yaw);
}

void LCD_show_UI6(void)
{
	MPU6050_Calculation();
	OLED_Clear();
	Show_MPU6050_UI();

	OLED_Update();
}

uint8_t hour,min,sec;
void Show_StopWatch_UI(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(32,20,OLED_8X16,"%02d:%02d:%02d",hour,min,sec);
	OLED_ShowString(8,44,"开始",OLED_8X16);
	OLED_ShowString(48,44,"停止",OLED_8X16);
	OLED_ShowString(88,44,"清除",OLED_8X16);
}

uint8_t start_timing_flag;
void StopWatch_Tick(void)
{
	static uint16_t Count;
	Count++;
	if(Count>=1000)
	{
		Count=0;
			if(start_timing_flag==1)
		{
			sec++;
			if(sec>=60)
			{
				sec=0;
				min++;
				if(min>=60)
				{
					min=0;
					hour++;
					if(hour>99)hour=0;
				}
			}
		}
	}
}

void LCD_show_UI7(void)
{
	OLED_Clear();
	Show_StopWatch_UI();

	switch(Rse7_flag)
	{
		case 0:
			OLED_ReverseArea(0,0,16,16);
			break;
		case 1:
			OLED_ReverseArea(8,44,24,16);
			break;
		case 2:
			OLED_ReverseArea(48,44,24,16);
			break;
		case 3:
			OLED_ReverseArea(88,44,24,16);
			break;
	}

	OLED_Update();
}

void Show_Game_UI(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(0,16,"谷歌小恐龙",OLED_8X16);
}

void LCD_show_UI8(void)
{
	OLED_Clear();
	Show_Game_UI();
	switch(Rse8_flag)
	{
		case 0:
			OLED_ReverseArea(0,0,16,16);
		break;
		case 1:
			OLED_ReverseArea(0,16,40,16);
		break;
	}
	OLED_Update();
}

void LCD_show(void)
{
	Key_scan();

	switch(lcd_mode)
	{
		case 0:
			LCD_show_UI0();
			break;
		case 1:
			LCD_show_UI1();
			break;
		case 2:
			LCD_show_UI2();
			break;
		case 3:
			LCD_show_UI3();
			break;
		case 4:
			LCD_show_UI4();
			break;
		case 5:
			LCD_show_UI5();
			break;
		case 6:
			LCD_show_UI6();
			break;
		case 7:
			LCD_show_UI7();
			break;
		case 8:
			LCD_show_UI8();
			break;
	}
}
