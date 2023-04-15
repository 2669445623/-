#include "screen.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "gpio.h"
#include "fonts.h"
#include "ssd1306.h"
#include <stdio.h>
#include "bitmap.h"
#include "duoji.h"
extern uint8_t s[6];
char date[7]={0};
int biaozhifu=0;
extern int success;
//char date1,date2,date3,date4,date5,date6,date7;

void xianshi_1(void const * argument)
{

	MX_I2C1_Init();
	SSD1306_Init();
//	SSD1306_Fill(0);
	SSD1306_UpdateScreen();
  for(;;)
  {
	
		if(success==1)
		{
		SSD1306_GotoXY(1,1);
		sprintf(&date[0],"%d",s[0]);
		SSD1306_Puts(&date[0],&Font_16x26,1);
		
		SSD1306_GotoXY(20,1);
		sprintf(&date[1],"%d",s[1]);
		SSD1306_Puts(&date[1],&Font_16x26,1);
		
		SSD1306_GotoXY(40,1);
		sprintf(&date[2],"%d",s[2]);
		SSD1306_Puts(&date[2],&Font_16x26,1);
	
		SSD1306_GotoXY(60,1);
		SSD1306_Puts("+",&Font_16x26,1);
		
		SSD1306_GotoXY(1,30);
		sprintf(&date[3],"%d",s[3]);
		SSD1306_Puts(&date[3],&Font_16x26,1);
		
		SSD1306_GotoXY(20,30);
		sprintf(&date[4],"%d",s[4]);
		SSD1306_Puts(&date[4],&Font_16x26,1);
		
		SSD1306_GotoXY(40,30);
		sprintf(&date[5],"%d",s[5]);
		SSD1306_Puts(&date[5],&Font_16x26,1);
		SSD1306_UpdateScreen();
		success=0;
		}
		
    osDelay(1);
  }
  /* USER CODE END Start_screen_Task */
}
