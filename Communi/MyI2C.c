#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue); //指定PB10为SCL控制线，枚举类型输入，0低电平，1高电平
	Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);  //指定PB11为SDA控制线同上
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);//读取SDA上的电平
	Delay_us(10);
	return BitValue;
}

void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出只能主动拉低，一个设备低则为低
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);
}

void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);//为了兼容重复起始条件先释放SDA,防止输出终止
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);//在SCL为高电平时，拉低SDA为起始条件，再拉低SCL为第一个数据输入做准备
}

void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);//先把SDA拉低，释放SCL为停止条件做准备，SCL高电平时释放SDA为停止条件
}

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i=0;i<8;i++)
	{
	MyI2C_W_SDA(Byte & (0x80>>i));//将1000 0000逻辑右移，与输入数据取与，每次得一位
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);//取得一位后先把SCL拉高读取，再拉低准备取下一位
	}
}//每次释放SCL只能读取一位有效电平，通过移位读取每次读取一位在拼接即可实现字节发送

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i,Byte = 0x00;//初始数据
	MyI2C_W_SDA(1);//释放SDA把SDA控制权交给从机
	for(i=0;i<8;i++)
	{
		MyI2C_W_SCL(1);//拉高SCL锁住数据
		if (MyI2C_R_SDA()==1){Byte|=(0x80>>i);}//读取到该位为1时改写一位Byte
		MyI2C_W_SCL(0);//拉低SCL准备读取下一位
	}
	return Byte;
}//SDA释放交给从机再通过改变SCL电平逐位读取

void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
	//写入应答位，SCL拉高读取，为1则从机停止传数据，再拉低SCL为停止做准备
}

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);//拉高SDA交给从机，拉高SCL锁住，读取应答，再拉低SCL为后来准备
	return AckBit;
}



