#include "stm32f10x.h"                  // Device header
#include "DELAY.h"
#include "Setime.h"
#include "LED.h"
#include "menu.h"

uint8_t Key_Num,Rse1_flag,Rse2_flag,Rse3_flag,Rse5_flag,Rse7_flag,lcd_mode,time_cg,MPU_S,mpu_flag;
uint8_t Rse8_flag;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if(Key_Num)
	{
		Temp=Key_Num;
		Key_Num=0;
		return Temp;
	}
	else
	{
		return 0;
	}
}

uint8_t Key_GetState(void)
{
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
	{
		return 1;
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
	{
		return 2;
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

void Key_Tick(void)
{
	static uint8_t Count;
	static uint8_t CurrentState,PreState;
	Count++;
	if(Count>=20)
	{
		Count=0;
		PreState=CurrentState;
		CurrentState=Key_GetState();
		if(PreState!=0&&CurrentState==0)
		{
			Key_Num=PreState;
		}
	}
}

void Key_scan_UI0(uint8_t KeyNum)
{
	switch(KeyNum)
	{
		case 1:
			Rse1_flag^=1;
			break;
		case 3:
			if(Rse1_flag==1)
			{
				lcd_mode^=1;
			}
			else if(Rse1_flag==0)
			{
				lcd_mode=4;
				target_selection=0;
			}
			break;
	}
}

void Key_scan_UI1(uint8_t KeyNum)
{
	switch(KeyNum)
	{
		case 1:
			Rse2_flag^=1;
			break;
		case 3:
				if(Rse2_flag==0)
				{
					lcd_mode=0;
				}
		
			else if(Rse2_flag==1)
			{
				lcd_mode=2;
				Rse3_flag=0;
				time_cg=0;
			}
			break;
	}
}

void Key_scan_UI2(uint8_t KeyNum)
{
	switch(KeyNum)
	{
		case 1:
			if(time_cg==1&&Rse3_flag>0)
			{
				Change_RTC_Time(1);
			}
			else
			{
				Rse3_flag--;
				if(Rse3_flag>3) Rse3_flag=3;
			}
			break;
		case 2:
			if(time_cg==1&&Rse3_flag>0)
			{
				Change_RTC_Time(2);
			}
			else
			{
				Rse3_flag++;
				if(Rse3_flag>3)
				{
					lcd_mode=3;
					Rse3_flag=0;
				}
			}
			break;
		case 3:
			if(Rse3_flag==0)
			{
				lcd_mode=1;
			}
			else
			{
				time_cg^=1;
			}
			break;
	}
}

void Key_scan_UI3(uint8_t KeyNum)
{
	switch(KeyNum)
	{
		case 1:
			if(time_cg==1)
			{
				Change_RTC_Time(1);
			}
			else
			{
				Rse3_flag--;
				if(Rse3_flag>2)
				{
					lcd_mode=2;
					Rse3_flag=3;
				}
			}
			break;
		case 2:
			if(time_cg==1)
			{
				Change_RTC_Time(2);
			}
			else
			{
				Rse3_flag++;
				if(Rse3_flag>2) Rse3_flag=0;
			}
			break;
		case 3:
			time_cg^=1;
			break;
	}
}

void Key_scan_UI4(uint8_t KeyNum)
{
	switch(KeyNum)
	{
		case 1:
			target_selection++;if(target_selection>6)target_selection=0;
			break;
		case 2:
			if(target_selection>0)
			{
				target_selection--;
			}
			break;
		case 3:
			if(target_selection==0)
			{
				lcd_mode=0;
			}
			else if(target_selection==2)
			{
				lcd_mode=5;
			}
			else if(target_selection==3)
			{
				lcd_mode=6;
				mpu_flag=1;
				MPU_S=6;
			}
			else if(target_selection==1)
			{
				lcd_mode=7;
			}
			else if(target_selection==4)
			{
				lcd_mode=8;
			}
			break;
	}
}

void Key_scan_UI5(uint8_t KeyNum)
{
	switch(KeyNum)
	{
		case 1:
		Rse5_flag++; if(Rse5_flag>2) Rse5_flag=0;
		break;
		case 3:
			if(Rse5_flag==0)
			{
			lcd_mode=4;
			}
			else if(Rse5_flag==1)
			{
				LED1_OFF();
			}
			else
			{
				LED1_ON();
			}
		break;
	}
}


void Key_scan_UI6(uint8_t KeyNum)
{
	switch(KeyNum)
	{
		case 3:
			mpu_flag=0;
		lcd_mode=4;
		break;
	}
		
}


void Key_scan_UI7(uint8_t KeyNum)
{
	switch(KeyNum)
	{
		case 1:
			if(Rse7_flag<3) Rse7_flag++;
			break;
		case 2:
			if(Rse7_flag>0) Rse7_flag--;
			break;
		case 3:
			if(Rse7_flag==0)
			{
				lcd_mode=4;
			}
			else if(Rse7_flag==1)
			{
				start_timing_flag=1;
			}
			else if(Rse7_flag==2)
			{
				start_timing_flag=0;
			}
			else if(Rse7_flag==3)
			{
				start_timing_flag=0;
				hour=0;
				min=0;
				sec=0;
			}
			break;
	}
}

void Key_scan_UI8(uint8_t KeyNum)
{
	switch(KeyNum)
	{
		case 1:
			Rse8_flag^=1;
			break;
		case 3:
			if(Rse8_flag==0)
			{
			lcd_mode=4;
			}
		break;
	}
}

void Key_scan(void)
{
	uint8_t KeyNum=Key_GetNum();
	
	switch(lcd_mode)
	{
		case 0:
			Key_scan_UI0(KeyNum);
			break;
		case 1:
			Key_scan_UI1(KeyNum);
			break;
		case 2:
			Key_scan_UI2(KeyNum);
			break;
		case 3:
			Key_scan_UI3(KeyNum);
			break;
		case 4:
			Key_scan_UI4(KeyNum);
			break;
		case 5:
			Key_scan_UI5(KeyNum);
			break;
		case 6:
			Key_scan_UI6(KeyNum);
			break;
		case 7:
			Key_scan_UI7(KeyNum);
			break;
		case 8:
			Key_scan_UI8(KeyNum);
			break;
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		Key_Tick();
		if(mpu_flag) MPU_S++;
		StopWatch_Tick();
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}