#include "main.h"

void BSP_Init(void)
{
    SystemInit();
	SysTick_init();		
	MYLCD_Init();
	USART1_Init();
	Normal_Init();
	DAC_Initialize();
	DAC_Output(1, 2048);
	DAC_Output(2, 2048);
}

void SysTick_init(void)
{
    SysTick_Config(72000000/OS_TICKS_PER_SEC);
}


