#include "dac.h"
#include "stm32f10x.h"

void DAC_Initialize(void)
{
	DAC_InitTypeDef DAC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	
	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_Cmd(DAC_Channel_2, ENABLE);
	
	DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
	DAC_SoftwareTriggerCmd(DAC_Channel_2,ENABLE);
}

void DAC_Output(unsigned char Channel, unsigned short Value)
{
	if(Channel == 1) {
		DAC_SetChannel1Data(DAC_Align_12b_R,Value);
		DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
	}
	else {
		DAC_SetChannel2Data(DAC_Align_12b_R,Value);
		DAC_SoftwareTriggerCmd(DAC_Channel_2,ENABLE);
	}
}
