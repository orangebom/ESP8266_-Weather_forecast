#ifndef __OLED_H__
#define __OLED_H__

#include "gpio.h"

#define OLED_SCLK_Clr() gpio_output_set(0, BIT12, BIT12, 0) //scl
#define OLED_SCLK_Set() gpio_output_set(BIT12, 0, BIT12, 0)

#define OLED_SDIN_Clr() gpio_output_set(0, BIT13, BIT13, 0) //SDA
#define OLED_SDIN_Set() gpio_output_set(BIT13, 0, BIT13, 0)


void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_WR_Byte(unsigned dat,unsigned cmd);
void OLED_Display_On(void);
void IIC_Start();
void IIC_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack();
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_Clear(void);

#endif
