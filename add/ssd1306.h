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
 这款SSD1306液晶显示器使用I2C进行通信库的功能是绘制直线、矩形和圆形。它还允许您使用库中提供的适当函数绘制文本和字符。默认的引出线
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
 * @brief  SSD1306 color enumeration  SSD1306颜色枚举
 */
typedef enum {
	SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel 黑色，无像素*/
	SSD1306_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD 设置像素。颜色取决于LCD */
} SSD1306_COLOR_t;



/**
 * @brief  Initializes SSD1306 LCD
 * @param  None
 * @retval Initialization status:
 *           - 0: LCD was not detected on I2C port
 *           - > 0: LCD initialized OK and ready to use 初始化SSD1306液晶没有一个初始化状态:O: LCD未检测到I2C端口-> 0:LCD初始化OK，准备使用
 */
uint8_t SSD1306_Init(void);

/** 
 * @brief  Updates buffer from internal RAM to LCD 更新缓冲区从内部RAM到LCD这个函数必须被调用
 * @note   This function must be called each time you do some changes to LCD, to update buffer from RAM to LCD   每次你对LCD做一些改变，以更新缓冲从RAM到LCD
 * @param  None
 * @retval None
 */
void SSD1306_UpdateScreen(void);

/**
 * @brief  Toggles pixels invertion inside internal RAM 在内部RAM内切换像素反转
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen必须在那之后被调用，以便看到更新的LCD屏幕
 * @param  None
 * @retval None
 */
void SSD1306_ToggleInvert(void);

/** 
 * @brief  Fills entire LCD with desired color 用所需的颜色填充整个用于填充屏幕的颜色。
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen必须在那之后被调用，以便看到更新的LCD屏幕颜色:
 * @param  Color: Color to be used for screen fill. This parameter can be a value of @ref SSD1306_COLOR_t enumeration这个参数可以是@ref SSD1306 COLOR枚举的值
 * @retval None
 */
void SSD1306_Fill(SSD1306_COLOR_t Color);

/**
 * @brief  Draws pixel at desired location  在所需位置绘制像素 颜色:用于填充屏幕的颜色。
 * @note   @ref SSD1306_UpdateScreen() must called after that in order to see updated LCD screen  必须在那之后被调用，以便看到更新的LCD屏幕:
 * @param  x: X location. This parameter can be a value between 0 and SSD1306_WIDTH - 1  x的位置。取值范围为O ~ SSD1306 WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SSD1306_HEIGHT - 1  y: y位置。取值范围为O ~ SSD1306 HEIGHT - 1
 * @param  color: Color to be used for screen fill. This parameter can be a value of  这个参数可以是@ref SSD1306_cOLOR_t枚举的值
 * @retval None
 */
void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color);

/**
 * @brief  Sets cursor pointer to desired location for strings 为字符串设置光标指针到所需的位置
 * @param  x: X location. This parameter can be a value between 0 and SSD1306_WIDTH - 1  x的位置。取值范围为O ~ SSD1306 WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SSD1306_HEIGHT - 1  y: y位置。取值范围为O ~ SSD1306 HEIGHT - 1
 * @retval None
 */
void SSD1306_GotoXY(uint16_t x, uint16_t y);

/**
  将字符放入内部RAM@ref SSD1306_UpdateScreen()必须在那之后被调用，以便看到更新的LCD屏幕ch:要写的字*字体:
	指向@ref FontDef_t结构体使用的字体
	颜色:用于绘画的颜色。该参数可以是@ref SSD1306_COLOR_t enumeration Character written的值
 * @brief  Puts character to internal RAM
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  ch: Character to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval Character written
 */
char SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color);

/**
  将字符串放入内部RAM@ref SSD1306_Updatescreen()必须在那之后被调用，以便看到更新的LCD屏幕
	*str:要写入的字符串*字体:指向@ref FontDef_t结构体使用的字体
	颜色:用于绘画的颜色。这个参数可以是@ref SSD1306_COLOR_t枚
	举0的值，或者是函数失败时的字符值
 * @brief  Puts string to internal RAM
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  *str: String to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval Zero on success or character value when function failed
 */
char SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color);

/**
   在LCD上绘制线
	 @ref SSD1306_UpdateScreen()必须在那之后被调用，以便看到更新的LCD屏幕x0:行x起点。
	 有效的输入是O到SSD1306 WIDTH -1yo: Y线的起点。有效输入是O到SSD1306高度- 1xl:直线x的终点。
	 有效的输入是0到SSD1306宽度- 1yl: Y线终点。有效的输入是o到SSD1306高度- 1
	 c:使用的颜色。这个参数可以是@ref SSD1306 COLOR枚举的值
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
   在LCD上绘制矩形
	 @ref SSD1306 UpdateScreen()必须在那之后被调用，以便看到更新的LCD屏幕x:左上角x的起点。
	 有效的输入是0到SSD1306宽度- 1y:左上角y起点。
	 有效输入是O到SSD1306高度- 1
	 w:以像素为单位的矩形宽度h:矩形高度，以像素为单位
	 c:颜色塞德你是an是@ref SSD1306 COLOR t enumerati的值
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
  在LCD上绘制填充的矩形
  @ref SSD1306_UpdateScreen()必须在那之后被调用，以便看到更新的LCD屏幕x:左上角x的起点。
  有效的输入是O到SSD1306 WIDTH -1
  y:左上角y起点。有效输入是O到SSD1306高度- 1
  w:以像素为单位的矩形宽度h:矩形高度，以像素为单位
  c:使用的颜色。这个参数可以是@ref SSD1306 COLOR枚举的值
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
   在LCD上绘制三角形
	 @ref SSD1306 UpdateScreen()必须在那之后被调用，以便看到更新的LCD屏幕
	 x1:第一个坐标X位置。有效的输入是0到SSD1306宽度- 1
	 yl:第一个坐标Y位置。有效的输入是o到SSD1306高度- 1
	 x2:第二坐标X位置。有效的输入是O到SSD1306 WIDTH - 1
	 y2:第二个坐标Y位置。有效的输入是o到SSD1306高度- 1
	 x3:第三坐标X位置。有效的输入是O到SSD1306 WIDTH -1
	 y3:第三坐标Y位置。有效输入是O到SSD1306高度- 1
	 c:使用的颜色。这个参数可以是@ref SSD1306 COLOR枚举的值
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
  将圆绘制到STM缓冲区
	@ref SSD1306_UpdateScreen()必须在那之后被调用，以便看到更新的LCD屏幕
	x: x位置的圆心。有效的输入是O到SSD1306 WIDTH - 1
	y:圆心的y位置。有效输入是O到SSD1306高度- 1
	r:以像素为单位的圆半径
	c:使用的颜色。这个参数可以是@ref SSD1306 COLOR枚举的值
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
  将填充圆绘制到STM缓冲区
	@ref SSD1306 UpdateScreen()必须在那之后被调用，以便看到更新的LCD屏幕
	x: x位置为圆心。有效的输入是0到SSD1306宽度- 1
	y:圆心的y位置。有效输入是O到SSD1306高度- 1
	r:以像素为单位的圆半径
	c:使用的颜色。这个参数可以是@ref SSD1306 COLOR枚举的值
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
  向slave写入多个字节*
	I2Cx: I2C使用
	address: 7位从地址，左对齐，使用7:1位，LSB位不使用
	reg:寄存器写入
	*data:指向数据数组的指针，将其写入slave
	Count:写入的字节数
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
  绘制位图
	X:开始绘图的X位置
	Y:开始绘图的Y位置
	*bitmap:指向位图的指针
	W:图像的宽度
	H:图像的高度颜色:1->白/蓝，0->黑
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
