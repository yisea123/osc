#ifndef __OSC_H
#define __OSC_H

#include "ucos_ii.h"
#include "GUI.h"

/*---------------------------------------------------------------------------------
								define
---------------------------------------------------------------------------------*/
#define OSC_DELAY(x) GUI_Delay(x)

/*---------------------------------------------------------------------------------
								Datatype
---------------------------------------------------------------------------------*/
typedef enum OSCTimeEnum{
	OSCTime_1US,   OSCTime_2US,   OSCTime_5US,
	OSCTime_10US,  OSCTime_20US,  OSCTime_50US,
	OSCTime_100US, OSCTime_200US, OSCTime_500US, 
	OSCTime_1MS,   OSCTime_2MS,   OSCTime_5MS,
	OSCTime_10MS,  OSCTime_20MS,  OSCTime_50MS, 
	OSCTime_100MS, OSCTime_200MS, OSCTime_500MS
}OSCTimeEnum;

typedef enum OSCVoltageEnum {
	OSCVoltage_10V,   OSCVoltage_5V,    OSCVoltage_2V,
	OSCVoltage_1V,    OSCVoltage_500MV, OSCVoltage_200MV,
	OSCVoltage_100MV, OSCVoltage_50MV
} OSCVoltageEnum;

typedef enum OSCTrigEnum {
	OSCTrig_Nnoe, 
	OSCTrig_CH1Falling, OSCTrig_CH1Rising,
	OSCTrig_CH2Falling, OSCTrig_CH2Rising 
} OSCTrigEnum;

typedef struct OSC_Struct {
	INT8U 			CHEnable[3];    /* [0]:CH1,  [1]:CH2,   [2]:MATH */
	FP32  			CHFrequency[2]; /* [0]:CH1,  [0]:CH2 */
	FP32 			CHVoltage[3];
	OSCTimeEnum 	CHTimeSel;
	OSCVoltageEnum  CHVoltageSel[3];
	INT16U          CHHOffset[3];
	INT16U          CHVOffset[3];
	OSCTrigEnum     CHTrig;          
} OSC_Struct;


/*---------------------------------------------------------------------------------
								Function
---------------------------------------------------------------------------------*/
void OSC_StructInit(void);
void OSC_GetADData(INT8S *Buf1, INT8S *Buf2);
void OSC_GetFrequency(INT16U *Buf);
void OSC_Draw(INT8S *Buf1, INT8U CH1En, INT8S *Buf2, INT8U CH2En, INT8S *Buf3, INT8U MathEn);
char *Enum2String(INT8U Type, INT8U tmp);

#endif










