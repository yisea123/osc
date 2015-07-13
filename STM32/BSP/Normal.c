#include "Normal.h"
#include "stm32f10x.h"
#include "delay.h"

void Normal_Init(void)
{
	GPIO_InitTypeDef GPIO;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_8;
	GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO);
	
	GPIO.GPIO_Pin   = 0xFFFF; /* SRAM data */
	GPIO.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO);
	
	GPIO.GPIO_Pin   = GPIO_Pin_1; /* MCU Read */
	GPIO_Init(GPIOE, &GPIO);
	
	GPIO.GPIO_Pin   = GPIO_Pin_0; /* Sample Finish */
	GPIO.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO);
	
	GPIO.GPIO_Pin   = GPIO_Pin_3; /* CPLD Reset */
	GPIO_Init(GPIOC, &GPIO);
	
	GPIO.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2; /* CH2 GainSel */
	GPIO_Init(GPIOC, &GPIO);

	MCURead_H;
	
	
	RCC->APB2ENR|=1<<4;      //使能PORTC时钟 
	RCC->APB2ENR|=1<<2;      //使能PORTA时钟
	RCC->APB2ENR|=1<<0;      //使能AFIO时钟

	GPIOA->CRL&=0XFFF00000; //PA0,1,2,3,4设置成输出  
	GPIOA->CRL|=0X00033333;

	GPIOC->CRH&=0X000FFFFF; //PC13,14,15设置成输出 2MHz 推挽输出  
	GPIOC->CRH|=0X22200000;

	PWR->CR|=1<<8; //取消备份区写保护
	RCC->BDCR&=0xFFFFFFFE; //外部低俗振荡器关闭 PC14，PC15成为普通IO  
	BKP->CR&=0xFFFFFFFE;  //侵入检测TAMPER引脚作为通用IO口使用
	PWR->CR&=0xFFFFFEFF; //备份区写保护 
	
	GPIO.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5; /* DAC output */
	GPIO.GPIO_Mode  = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO);

	GPIO.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5; /* Debug LEDs */
	GPIO.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO);
	
	MCUReadFre_L;	
	CH1GainSel(0);
	CH2GainSel(0);
}

void TimeSel(unsigned char Type)
{
	GPIOA->ODR &= ~0x000F;
	GPIOA->ODR |= Type;
	if(Type & 0x10) GPIOA->BSRR = 0x0010;
	else            GPIOA->BRR  = 0x0010;
}

void CH1GainSel(unsigned char Type)
{
	GPIOC->ODR &= ~0xE000;
	GPIOC->ODR |= (unsigned short)Type << 13;
}

void CH2GainSel(unsigned char Type)
{
	GPIOC->ODR &= ~0x07;
	GPIOC->ODR |= Type;
}

