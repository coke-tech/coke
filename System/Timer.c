#include "stm32f10x.h"                  // Device header



void Timer_Init(void)//独立于 CPU 的硬件计时 / 计数模块，无需占用 CPU 资源
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//相当于给外设通电
	
	TIM_InternalClockConfig(TIM2);//选择时钟源为内部时钟，72M
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =TIM_CKD_DIV1;//时钟分频因子，内部时钟与外部捕获时钟的比值
	TIM_TimeBaseInitStructure.TIM_CounterMode =TIM_CounterMode_Up;//向上计数，0-ARR
	TIM_TimeBaseInitStructure.TIM_Period =100-1;//自动重装寄存器（ARR）的值，计数器的 “溢出阈值”；记数一万个周期后重装
	TIM_TimeBaseInitStructure.TIM_Prescaler =720-1;//配置预分频器，72M/36000=2000
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter =0;//重复计数器值，仅高级定时器（TIM1/TIM8） 有效，用于 “多次 PWM 周期后才触发更新事件”；
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//使配置生效传递配置参数
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//清除标志位，防止一开始就触发中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//相当于配置中断线，允许TIM产生更新中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);//启动计数器，正式启动选择的时钟源
}


