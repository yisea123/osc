#ifndef __MYLCD_H
#define __MYLCD_H


#define RGB2BIT16(x1, x2, x3) ((unsigned short)(x1 << 11) | (unsigned short)(x2 << 6) | (unsigned short)(x3))
#define MyLCDRed    0xf800  	//��
#define MyLCDYellow 0xff0e	//��
#define MyLCDGreen  0x07e0	//��
#define MyLCDCyan   0x70ff 	//��
#define MyLCDBlue   0x00f1	//��
#define MyLCDPurple 0x8ff1	//��
#define MyLCDGGray   0xb7fe  	//��
#define MyLCDBlack  0x0000
#define MyLCDWhite  0xffff

#define TouchDown (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0)

void MYLCD_Init(void);
void AddressSet(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void AddressSetH(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_RAM(unsigned int Data);
void LCD_REG(unsigned int Cmd);

void LCDDirH(void);
void LCDDirV(void);

void Touch_Init(void);
unsigned char Touch_GetXY(unsigned int *x, unsigned int *y);
unsigned short LCD_ReadPoint(unsigned int x, unsigned int y);

#endif
