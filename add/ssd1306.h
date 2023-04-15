/**
 * original author:  Tilen Majerle<tilen@majerle.eu>
 * modification for STM32f10x: Alexander Lutsai<s.lyra@ya.ru>

   ----------------------------------------------------------------------
   	Copyright (C) Alexander Lutsai, 2016
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
 */
#ifndef SSD1306_H
#define SSD1306_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 * This SSD1306 LCD uses I2C for communication
 *
 * Library features functions for drawing lines, rectangles and circles.
 *
 * It also allows you to draw texts and characters using appropriate functions provided in library.
 *
 * Default pinout
 *
 ���SSD1306Һ����ʾ��ʹ��I2C����ͨ�ſ�Ĺ����ǻ���ֱ�ߡ����κ�Բ�Ρ�����������ʹ�ÿ����ṩ���ʵ����������ı����ַ���Ĭ�ϵ�������
SSD1306    |STM32F10x    |DESCRIPTION

VCC        |3.3V         |
GND        |GND          |
SCL        |PB6          |Serial clock line
SDA        |PB7          |Serial data line
 */

#include "stm32f4xx_hal.h"

#include "fonts.h"

#include "stdlib.h"
#include "string.h"


/* I2C address */
#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR         0x78
//#define SSD1306_I2C_ADDR       0x7A
#endif

/* SSD1306 settings */
/* SSD1306 width in pixels */
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH            128
#endif
/* SSD1306 LCD height in pixels */
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT           64
#endif

/**
 * @brief  SSD1306 color enumeration  SSD1306��ɫö��
 */
typedef enum {
	SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel ��ɫ��������*/
	SSD1306_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD �������ء���ɫȡ����LCD */
} SSD1306_COLOR_t;



/**
 * @brief  Initializes SSD1306 LCD
 * @param  None
 * @retval Initialization status:
 *           - 0: LCD was not detected on I2C port
 *           - > 0: LCD initialized OK and ready to use ��ʼ��SSD1306Һ��û��һ����ʼ��״̬:O: LCDδ��⵽I2C�˿�-> 0:LCD��ʼ��OK��׼��ʹ��
 */
uint8_t SSD1306_Init(void);

/** 
 * @brief  Updates buffer from internal RAM to LCD ���»��������ڲ�RAM��LCD����������뱻����
 * @note   This function must be called each time you do some changes to LCD, to update buffer from RAM to LCD   ÿ�����LCD��һЩ�ı䣬�Ը��»����RAM��LCD
 * @param  None
 * @retval None
 */
void SSD1306_UpdateScreen(void);

/**
 * @brief  Toggles pixels invertion inside internal RAM ���ڲ�RAM���л����ط�ת
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen��������֮�󱻵��ã��Ա㿴�����µ�LCD��Ļ
 * @param  None
 * @retval None
 */
void SSD1306_ToggleInvert(void);

/** 
 * @brief  Fills entire LCD with desired color ���������ɫ����������������Ļ����ɫ��
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen��������֮�󱻵��ã��Ա㿴�����µ�LCD��Ļ��ɫ:
 * @param  Color: Color to be used for screen fill. This parameter can be a value of @ref SSD1306_COLOR_t enumeration�������������@ref SSD1306 COLORö�ٵ�ֵ
 * @retval None
 */
void SSD1306_Fill(SSD1306_COLOR_t Color);

/**
 * @brief  Draws pixel at desired location  ������λ�û������� ��ɫ:���������Ļ����ɫ��
 * @note   @ref SSD1306_UpdateScreen() must called after that in order to see updated LCD screen  ��������֮�󱻵��ã��Ա㿴�����µ�LCD��Ļ:
 * @param  x: X location. This parameter can be a value between 0 and SSD1306_WIDTH - 1  x��λ�á�ȡֵ��ΧΪO ~ SSD1306 WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SSD1306_HEIGHT - 1  y: yλ�á�ȡֵ��ΧΪO ~ SSD1306 HEIGHT - 1
 * @param  color: Color to be used for screen fill. This parameter can be a value of  �������������@ref SSD1306_cOLOR_tö�ٵ�ֵ
 * @retval None
 */
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);

/**
 * @brief  Sets cursor pointer to desired location for strings Ϊ�ַ������ù��ָ�뵽�����λ��
 * @param  x: X location. This parameter can be a value between 0 and SSD1306_WIDTH - 1  x��λ�á�ȡֵ��ΧΪO ~ SSD1306 WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SSD1306_HEIGHT - 1  y: yλ�á�ȡֵ��ΧΪO ~ SSD1306 HEIGHT - 1
 * @retval None
 */
void SSD1306_GotoXY(uint16_t x, uint16_t y);

/**
  ���ַ������ڲ�RAM@ref SSD1306_UpdateScreen()��������֮�󱻵��ã��Ա㿴�����µ�LCD��Ļch:Ҫд����*����:
	ָ��@ref FontDef_t�ṹ��ʹ�õ�����
	��ɫ:���ڻ滭����ɫ���ò���������@ref SSD1306_COLOR_t enumeration Character written��ֵ
 * @brief  Puts character to internal RAM
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  ch: Character to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval Character written
 */
char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color);

/**
  ���ַ��������ڲ�RAM@ref SSD1306_Updatescreen()��������֮�󱻵��ã��Ա㿴�����µ�LCD��Ļ
	*str:Ҫд����ַ���*����:ָ��@ref FontDef_t�ṹ��ʹ�õ�����
	��ɫ:���ڻ滭����ɫ���������������@ref SSD1306_COLOR_tö
	��0��ֵ�������Ǻ���ʧ��ʱ���ַ�ֵ
 * @brief  Puts string to internal RAM
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  *str: String to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval Zero on success or character value when function failed
 */
char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color);

/**
   ��LCD�ϻ�����
	 @ref SSD1306_UpdateScreen()��������֮�󱻵��ã��Ա㿴�����µ�LCD��Ļx0:��x��㡣
	 ��Ч��������O��SSD1306 WIDTH -1yo: Y�ߵ���㡣��Ч������O��SSD1306�߶�- 1xl:ֱ��x���յ㡣
	 ��Ч��������0��SSD1306���- 1yl: Y���յ㡣��Ч��������o��SSD1306�߶�- 1
	 c:ʹ�õ���ɫ���������������@ref SSD1306 COLORö�ٵ�ֵ
 * @brief  Draws line on LCD
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x0: Line X start point. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y0: Line Y start point. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  x1: Line X end point. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y1: Line Y end point. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c);

/**
   ��LCD�ϻ��ƾ���
	 @ref SSD1306 UpdateScreen()��������֮�󱻵��ã��Ա㿴�����µ�LCD��Ļx:���Ͻ�x����㡣
	 ��Ч��������0��SSD1306���- 1y:���Ͻ�y��㡣
	 ��Ч������O��SSD1306�߶�- 1
	 w:������Ϊ��λ�ľ��ο��h:���θ߶ȣ�������Ϊ��λ
	 c:��ɫ��������an��@ref SSD1306 COLOR t enumerati��ֵ
 * @brief  Draws rectangle on LCD
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: Top left X start point. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y: Top left Y start point. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  w: Rectangle width in units of pixels
 * @param  h: Rectangle height in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);

/**
  ��LCD�ϻ������ľ���
  @ref SSD1306_UpdateScreen()��������֮�󱻵��ã��Ա㿴�����µ�LCD��Ļx:���Ͻ�x����㡣
  ��Ч��������O��SSD1306 WIDTH -1
  y:���Ͻ�y��㡣��Ч������O��SSD1306�߶�- 1
  w:������Ϊ��λ�ľ��ο��h:���θ߶ȣ�������Ϊ��λ
  c:ʹ�õ���ɫ���������������@ref SSD1306 COLORö�ٵ�ֵ
 * @brief  Draws filled rectangle on LCD
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: Top left X start point. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y: Top left Y start point. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  w: Rectangle width in units of pixels
 * @param  h: Rectangle height in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);

/**
   ��LCD�ϻ���������
	 @ref SSD1306 UpdateScreen()��������֮�󱻵��ã��Ա㿴�����µ�LCD��Ļ
	 x1:��һ������Xλ�á���Ч��������0��SSD1306���- 1
	 yl:��һ������Yλ�á���Ч��������o��SSD1306�߶�- 1
	 x2:�ڶ�����Xλ�á���Ч��������O��SSD1306 WIDTH - 1
	 y2:�ڶ�������Yλ�á���Ч��������o��SSD1306�߶�- 1
	 x3:��������Xλ�á���Ч��������O��SSD1306 WIDTH -1
	 y3:��������Yλ�á���Ч������O��SSD1306�߶�- 1
	 c:ʹ�õ���ɫ���������������@ref SSD1306 COLORö�ٵ�ֵ
 * @brief  Draws triangle on LCD
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x1: First coordinate X location. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y1: First coordinate Y location. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  x2: Second coordinate X location. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y2: Second coordinate Y location. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  x3: Third coordinate X location. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y3: Third coordinate Y location. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);

/**
  ��Բ���Ƶ�STM������
	@ref SSD1306_UpdateScreen()��������֮�󱻵��ã��Ա㿴�����µ�LCD��Ļ
	x: xλ�õ�Բ�ġ���Ч��������O��SSD1306 WIDTH - 1
	y:Բ�ĵ�yλ�á���Ч������O��SSD1306�߶�- 1
	r:������Ϊ��λ��Բ�뾶
	c:ʹ�õ���ɫ���������������@ref SSD1306 COLORö�ٵ�ֵ
 * @brief  Draws circle to STM buffer
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c);

/**
  �����Բ���Ƶ�STM������
	@ref SSD1306 UpdateScreen()��������֮�󱻵��ã��Ա㿴�����µ�LCD��Ļ
	x: xλ��ΪԲ�ġ���Ч��������0��SSD1306���- 1
	y:Բ�ĵ�yλ�á���Ч������O��SSD1306�߶�- 1
	r:������Ϊ��λ��Բ�뾶
	c:ʹ�õ���ɫ���������������@ref SSD1306 COLORö�ٵ�ֵ
 * @brief  Draws filled circle to STM buffer
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  x: X location for center of circle. Valid input is 0 to SSD1306_WIDTH - 1
 * @param  y: Y location for center of circle. Valid input is 0 to SSD1306_HEIGHT - 1
 * @param  r: Circle radius in units of pixels
 * @param  c: Color to be used. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c);



#ifndef ssd1306_I2C_TIMEOUT
#define ssd1306_I2C_TIMEOUT					20000
#endif

/**
 * @brief  Initializes SSD1306 LCD
 * @param  None
 * @retval Initialization status:
 *           - 0: LCD was not detected on I2C port
 *           - > 0: LCD initialized OK and ready to use
 */
void ssd1306_I2C_Init();

/**
 * @brief  Writes single byte to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  data: data to be written
 * @retval None
 */
void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data);

/**
  ��slaveд�����ֽ�*
	I2Cx: I2Cʹ��
	address: 7λ�ӵ�ַ������룬ʹ��7:1λ��LSBλ��ʹ��
	reg:�Ĵ���д��
	*data:ָ�����������ָ�룬����д��slave
	Count:д����ֽ���
 * @brief  Writes multi bytes to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  *data: pointer to data array to write it to slave
 * @param  count: how many bytes will be written
 * @retval None
 */
void ssd1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);

/**
  ����λͼ
	X:��ʼ��ͼ��Xλ��
	Y:��ʼ��ͼ��Yλ��
	*bitmap:ָ��λͼ��ָ��
	W:ͼ��Ŀ��
	H:ͼ��ĸ߶���ɫ:1->��/����0->��
 * @brief  Draws the Bitmap
 * @param  X:  X location to start the Drawing
 * @param  Y:  Y location to start the Drawing
 * @param  *bitmap : Pointer to the bitmap
 * @param  W : width of the image
 * @param  H : Height of the image
 * @param  color : 1-> white/blue, 0-> black
 */
void SSD1306_DrawBitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color);

// scroll the screen for fixed rows

void SSD1306_ScrollRight(uint8_t start_row, uint8_t end_row);


void SSD1306_ScrollLeft(uint8_t start_row, uint8_t end_row);


void SSD1306_Scrolldiagright(uint8_t start_row, uint8_t end_row);


void SSD1306_Scrolldiagleft(uint8_t start_row, uint8_t end_row);



void SSD1306_Stopscroll(void);


// inverts the display i = 1->inverted, i = 0->normal

void SSD1306_InvertDisplay (int i);






// clear the display

void SSD1306_Clear (void);


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
