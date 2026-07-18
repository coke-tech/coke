#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[] = {2026, 3, 8, 23, 17, 55};
void MyRTC_SetTime(void);

void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);//开启时钟解除保护
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		RCC_LSEConfig(RCC_LSE_ON);
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);//开启外部低速晶振，等待就绪标志位
		//32768HZ
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);//选择LSE作为RTC时钟源，给RTC供电
		
		RTC_WaitForSynchro();//等待 RTC 的日历寄存器（时间寄存器）与 APB1 总线同步
		RTC_WaitForLastTask();//等待上一次 RTC 操作（如配置、写入）完成
		
		RTC_SetPrescaler(32768 - 1);//预分频值
		RTC_WaitForLastTask();//等待分频配置完成
		
		MyRTC_SetTime();
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		RTC_WaitForSynchro();//读 RTC 前用，等待 RTC 时钟域同步，保证读取的时间 / 计数值准确
		RTC_WaitForLastTask();
	}
}
//用BKP判断是否为第一次配置时钟，选择外部低速晶振作为时钟源给RTC使用，把RTC寄存器与总线同步，配置1HZ频率，写入配置的时间


void MyRTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_date.tm_year = MyRTC_Time[0] - 1900;
	time_date.tm_mon = MyRTC_Time[1] - 1;
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min = MyRTC_Time[4];
	time_date.tm_sec = MyRTC_Time[5];
	
	time_cnt = mktime(&time_date) - 8 * 60 * 60;//计算得出的是本地时区，会主动加8小时
	
	RTC_SetCounter(time_cnt);//将校准好的时间写入RTC计时器
	RTC_WaitForLastTask();//配置结构体，把结构体地址写入函数得秒数，自动偏移八小时要修正
}

void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;//
	
	time_date = *localtime(&time_cnt);
	
	MyRTC_Time[0] = time_date.tm_year + 1900;
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}

