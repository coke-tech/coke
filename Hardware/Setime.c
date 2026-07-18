#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MyRTC.h"
#include "Key.h"

void Show_SetTime_FirstUI(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(0,16,OLED_8X16,"年:%4d",MyRTC_Time[0]);
	OLED_Printf(0,32,OLED_8X16,"月:%2d",MyRTC_Time[1]);
	OLED_Printf(0,48,OLED_8X16,"日:%2d",MyRTC_Time[2]);
}

void Show_SetTime_SecondUI(void)
{
	
	OLED_Printf(0,0,OLED_8X16,"时:%2d",MyRTC_Time[3]);
	OLED_Printf(0,16,OLED_8X16,"分:%2d",MyRTC_Time[4]);
	OLED_Printf(0,32,OLED_8X16,"秒:%2d",MyRTC_Time[5]);
}

void Change_RTC_Time(uint8_t keyNum)
{
	uint8_t index;

	if(lcd_mode == 2)
	{
		index = Rse3_flag - 1;
	}
	else if(lcd_mode == 3)
	{
		index = Rse3_flag + 3;
	}
	else
	{
		return;
	}

	if(keyNum == 1)
	{
		if(index == 0)
		{
			MyRTC_Time[0]++;
			if(MyRTC_Time[0] > 2099) MyRTC_Time[0] = 2000;
		}
		else if(index == 1)
		{
			MyRTC_Time[1]++;
			if(MyRTC_Time[1] > 12) MyRTC_Time[1] = 1;
		}
		else if(index == 2)
		{
			MyRTC_Time[2]++;
			if(MyRTC_Time[2] > 31) MyRTC_Time[2] = 1;
		}
		else if(index == 3)
		{
			MyRTC_Time[3]++;
			if(MyRTC_Time[3] > 23) MyRTC_Time[3] = 0;
		}
		else if(index == 4)
		{
			MyRTC_Time[4]++;
			if(MyRTC_Time[4] > 59) MyRTC_Time[4] = 0;
		}
		else if(index == 5)
		{
			MyRTC_Time[5]++;
			if(MyRTC_Time[5] > 59) MyRTC_Time[5] = 0;
		}
	}
	else if(keyNum == 2)
	{
		if(index == 0)
		{
			MyRTC_Time[0]--;
			if(MyRTC_Time[0] < 2000) MyRTC_Time[0] = 2099;
		}
		else if(index == 1)
		{
			MyRTC_Time[1]--;
			if(MyRTC_Time[1] < 1) MyRTC_Time[1] = 12;
		}
		else if(index == 2)
		{
			MyRTC_Time[2]--;
			if(MyRTC_Time[2] < 1) MyRTC_Time[2] = 31;
		}
		else if(index == 3)
		{
			MyRTC_Time[3]--;
			if(MyRTC_Time[3] > 23) MyRTC_Time[3] = 23;
		}
		else if(index == 4)
		{
			MyRTC_Time[4]--;
			if(MyRTC_Time[4] > 59) MyRTC_Time[4] = 59;
		}
		else if(index == 5)
		{
			MyRTC_Time[5]--;
			if(MyRTC_Time[5] > 59) MyRTC_Time[5] = 59;
		}
	}

	MyRTC_SetTime();
}
