#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "menu.h"
#include "Key.h"
#include "Setime.h"

int main(void)
{
	OLED_Init();
	
	comInit();
	while (1)
	{
		
		LCD_show();
		
	}
}
