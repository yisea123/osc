#include "mylcd.h"
#include "stm32f10x.h"
#include "delay.h"

#define Touch_PEN_Pin  GPIO_Pin_2
#define Touch_DOUT_Pin GPIO_Pin_3
#define Touch_DIN_Pin  GPIO_Pin_4
#define Touch_CS_Pin   GPIO_Pin_5
#define Touch_CLK_Pin  GPIO_Pin_6

#define Touch_PEN_H  GPIO_ReadInputDataBit(GPIOE, Touch_PEN_Pin)

#define Touch_DOUT   GPIO_ReadInputDataBit(GPIOE, Touch_DOUT_Pin)

#define Touch_DIN_H  GPIOE->BSRR = Touch_DIN_Pin
#define Touch_DIN_L  GPIOE->BRR  = Touch_DIN_Pin

#define Touch_CS_H   GPIOE->BSRR = Touch_CS_Pin
#define Touch_CS_L   GPIOE->BRR  = Touch_CS_Pin

#define Touch_CLK_H  GPIOE->BSRR = Touch_CLK_Pin
#define Touch_CLK_L  GPIOE->BRR  = Touch_CLK_Pin


void LCDPinInit(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;	
	GPIO_InitTypeDef GPIO_InitStructure;	    
	//使能FSMC外设时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
						 RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);  
	// 复用端口为FSMC接口 FSMC-D0--D15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
								GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
								GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);   

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
								GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
								GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);    
	//FSMC NE1  LCD片选
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//FSMC RS---LCD指令 指令/数据	切换
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
	GPIO_Init(GPIOD, &GPIO_InitStructure); 	


	//FSMC接口特性配置
	p.FSMC_AddressSetupTime = 0x02;
	p.FSMC_AddressHoldTime = 0x00;
	p.FSMC_DataSetupTime = 0x05;
	p.FSMC_BusTurnAroundDuration = 0x00;
	p.FSMC_CLKDivision = 0x00;
	p.FSMC_DataLatency = 0x00;
	p.FSMC_AccessMode = FSMC_AccessMode_B;


	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 		
	/* Enable FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE); 
}

void LCD_REG(unsigned int Cmd)
{
	*(__IO uint16_t *) ((uint32_t)0x60000000)= Cmd; 
}

void LCD_RAM(unsigned int Data)
{
	*(__IO uint16_t *) ((uint32_t)0x60020000)= Data;
}

void Write_Cmd_Data(unsigned int Cmd, unsigned int Data)
{
	LCD_REG(Cmd);
	LCD_RAM(Data);
}

/***********
 *   NAME: 设置显示的地址
 *  INPUT: 地址的起始和终止坐标
 * OUTPUT: NO
***********/
void AddressSet(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{  
	LCD_REG(0x2A); 
	LCD_RAM( x1>>8);
	LCD_RAM( x1&0xFF);
	LCD_RAM(x2>>8); 
	LCD_RAM(x2&0XFF);	 
	LCD_REG(0x2B); 
	LCD_RAM( y1>>8);
	LCD_RAM( y1&0xFF);
	LCD_RAM(y2>>8); 
	LCD_RAM(y2&0XFF);
	LCD_REG(0x2C); 
}

void AddressSetH(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{  

}

unsigned short LCD_ReadPoint(unsigned int x, unsigned int y)
{
	unsigned short tmp;
	LCD_REG(0x2A); 
	LCD_RAM( x>>8);
	LCD_RAM( x&0xFF);
	LCD_RAM(x>>8); 
	LCD_RAM(x&0XFF);	 
	LCD_REG(0x2B); 
	LCD_RAM( y>>8);
	LCD_RAM( y&0xFF);
	LCD_RAM(y>>8); 
	LCD_RAM(y&0XFF);
	LCD_REG(0x2E); 
	
	tmp =  *(__IO uint16_t *) ((uint32_t)0x60020000);
	tmp = *(__IO uint16_t *) ((uint32_t)0x60020000);
	return tmp;
}

/***********
 *   NAME: 液晶清屏
 *  INPUT: 清屏的颜色
 * OUTPUT: NO
***********/
void LCD_Clear(unsigned int Color)
{
	unsigned long i = 240 * 320;
	AddressSet(0, 0, 239, 319);
	while(i--) {
		LCD_RAM(Color);
	}
}

void LCDDirH(void)
{

}

void LCDDirV(void)
{

}

/***********
 *   NAME: 液晶的初始化
 *  INPUT: NO
 * OUTPUT: NO
***********/
void MYLCD_Init(void)
{
	Touch_Init();
	LCDPinInit();
	Delay_Ms(100);
		
	LCD_REG(0xF0);//PASSWD1
	LCD_RAM(0x5A);
	LCD_RAM(0x5A);

	LCD_REG(0xF1);//PASSWD2
	LCD_RAM(0x5A);
	LCD_RAM(0x5A);

	LCD_REG(0xF2);//DISCTL
	LCD_RAM(0x3B);
	LCD_RAM(0x48);//调整频率 0x48
	LCD_RAM(0x03);
	LCD_RAM(0x08);
	LCD_RAM(0x08);
	LCD_RAM(0x08);
	LCD_RAM(0x08);
	LCD_RAM(0x00);
	LCD_RAM(0x08);
	LCD_RAM(0x08);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x54);//0x54调整频率
	LCD_RAM(0x08);
	LCD_RAM(0x08);
	LCD_RAM(0x08);
	LCD_RAM(0x08);

	LCD_REG(0xF4);//PWRCTL
	LCD_RAM(0x09);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x3F);
	LCD_RAM(0x79);
	LCD_RAM(0x03);
	LCD_RAM(0x3F);
	LCD_RAM(0x79);
	LCD_RAM(0x03);

	LCD_REG(0xF5);//VCMCTL		
	LCD_RAM(0x00);
	LCD_RAM(0x5D);
	LCD_RAM(0x75);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x00);		
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x04);
	LCD_RAM(0x00);
	LCD_RAM(0x5D);
	LCD_RAM(0x75);

	LCD_REG(0xF6);//SRCCTL		
	LCD_RAM(0x04);
	LCD_RAM(0x00);
	LCD_RAM(0x08);
	LCD_RAM(0x03);
	LCD_RAM(0x01);
	LCD_RAM(0x00);
	LCD_RAM(0x01);
	LCD_RAM(0x00);

	LCD_REG(0xF7);//IFCTL
	LCD_RAM(0x48);
	LCD_RAM(0x80);
	LCD_RAM(0x10);
	LCD_RAM(0x02);
	LCD_RAM(0x00);

	LCD_REG( 0xF8);//PANELCTL
	LCD_RAM(0x11);
	LCD_RAM(0x00);


	LCD_REG(0xF9);//GAMMASEL
	LCD_RAM(0x24);	//red
	LCD_REG(0xFA);//PGAMMACTL
	LCD_RAM(0x0B);
	LCD_RAM(0x0B);
	LCD_RAM(0x05);
	LCD_RAM(0x01);
	LCD_RAM(0x0B);
	LCD_RAM(0x20);
	LCD_RAM(0x2C);
	LCD_RAM(0x13);
	LCD_RAM(0x21);
	LCD_RAM(0x24);
	LCD_RAM(0x30);
	LCD_RAM(0x32);
	LCD_RAM(0x24);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x01);


	LCD_REG(0xF9);//GAMMASEL
	LCD_RAM(0x22);	//green	
	LCD_REG(0xFA);//PGAMMACTL
	LCD_RAM(0x0B);
	LCD_RAM(0x0B);
	LCD_RAM(0x10);
	LCD_RAM(0x31);
	LCD_RAM(0x32);
	LCD_RAM(0x35);
	LCD_RAM(0x36);
	LCD_RAM(0x11);
	LCD_RAM(0x1D);
	LCD_RAM(0x23);
	LCD_RAM(0x2F);
	LCD_RAM(0x2F);
	LCD_RAM(0x24);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x01);


	LCD_REG(0xF9);//GAMMASEL
	LCD_RAM(0x21);	//blue	
	LCD_REG(0xFA);//PGAMMACTL
	LCD_RAM(0x0B);
	LCD_RAM(0x0B);
	LCD_RAM(0x1A);
	LCD_RAM(0x3A);
	LCD_RAM(0x3F);
	LCD_RAM(0x3F);
	LCD_RAM(0x3F);
	LCD_RAM(0x07);
	LCD_RAM(0x18);
	LCD_RAM(0x1F);
	LCD_RAM(0x28);
	LCD_RAM(0x1E);
	LCD_RAM(0x1A);
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x01);


	LCD_REG(0x3A);//COLMOD
	LCD_RAM(0x55);//Base Customer selection      
								//77H=24bits/pixel ; 
								//66H=18bits/pixel ;                              
								//55H=16bits/pixel ; 


	LCD_REG(0x36);//MADCTL
	LCD_RAM(0x00);

	LCD_REG(0x35);//TEON
	LCD_RAM(0x00);

	LCD_REG(0x2B);//PASET
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x01);
	LCD_RAM(0xDF);

	LCD_REG(0x2A);//CASET
	LCD_RAM(0x00);
	LCD_RAM(0x00);
	LCD_RAM(0x01);
	LCD_RAM(0x3F);

	LCD_REG(0x11);//SLPOUT
	Delay_Ms(120);
	LCD_REG(0x29);//DISPON	
}

void Touch_Init(void)
{
	GPIO_InitTypeDef GPIO;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO.GPIO_Pin   = Touch_DIN_Pin | Touch_CLK_Pin | Touch_CS_Pin;
	GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO);
	
	GPIO.GPIO_Pin   = Touch_DOUT_Pin | Touch_PEN_Pin; 
	GPIO.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &GPIO);
	
	Touch_CS_H;
	Touch_DIN_H;
	Touch_CLK_L;
}

void Touch_SendData(unsigned char Data)
{
	unsigned char i = 0;
	for(i = 0; i < 8; i++) {
		Touch_CLK_L;
		if(Data & (0x80 >> i)) Touch_DIN_H;
		else                   Touch_DIN_L;
		Touch_CLK_H;
	}
}

unsigned int Touch_ReadData(void)
{
	unsigned char i = 0;
	unsigned int tmp = 0;
	for(i = 0; i < 12; i++) {
		Touch_CLK_L;
		Touch_CLK_H;
		if(Touch_DOUT) tmp |= ((1 << 11) >> i);
	}
	return tmp;
}

void Touch_GetXY_One(unsigned int *x, unsigned int *y)
{	
	Touch_CS_L;
	Touch_SendData(0x90);
	Touch_CLK_H;
	Touch_CLK_L;
	Delay(0xf);
	*y = Touch_ReadData();

	Touch_SendData(0xD0);
	Touch_CLK_H;
	Touch_CLK_L;
	Delay(0xf);
	*x = Touch_ReadData();
	Touch_CS_H;
}

unsigned char Touch_GetXY(unsigned int *x, unsigned int *y) 
{
	unsigned int x1, y1, x2, y2, ty = 10;
	
	while(--ty) {
		Touch_GetXY_One(&x1, &y1);
		Touch_GetXY_One(&x2, &y2);
		if(x1 == x2 && y1 == y2) {
			*x = x1;
			*y = y1;
			break;
		}
	}                           
	return (ty > 0) ? 1 : 0;
}

