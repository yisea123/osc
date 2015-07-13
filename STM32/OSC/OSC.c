#include "OSC.h"
#include "Normal.h"
#include "main.h"
#include "MyLCD.h"

OSC_Struct OSC;

/*---------------------------------------------------------------------------------
								结构体初始化
---------------------------------------------------------------------------------*/
void OSC_StructInit(void)
{
	OSC.CHEnable[0] = 0;
	OSC.CHEnable[1] = 1;
	OSC.CHEnable[2] = 0;
	
	OSC.CHTimeSel = OSCTime_10US;
	
	OSC.CHVoltageSel[0] = OSCVoltage_10V;
	OSC.CHVoltageSel[1] = OSCVoltage_10V;
	OSC.CHVoltageSel[2] = OSCVoltage_10V;
		
	OSC.CHHOffset[0] = 0;
	OSC.CHHOffset[1] = 0;
	OSC.CHHOffset[2] = 0;
	OSC.CHVOffset[0] = 0;
	OSC.CHVOffset[1] = 0;
	OSC.CHVOffset[2] = 0;
	
	OSC.CHTrig = OSCTrig_Nnoe;
}
           

/*---------------------------------------------------------------------------------
								获取AD值
---------------------------------------------------------------------------------*/
void OSC_GetADData(INT8S *Buf1, INT8S *Buf2)
{
	INT16U i = 0, ty;
	INT16U CH1Base = 120, CH2Base = 118;
	float CH1Rate = 0.74, CH2Rate = 1;
	
	
	MCURead_L;
	MCUReadFre_L;
	CH1GainSel(OSC.CHVoltageSel[0]);
	CH2GainSel(OSC.CHVoltageSel[1]);
	CPLDReset(OSC.CHTimeSel, OSC.CHTrig);
	
	ty = 1000;
	while(!SampleFinish && --ty) {
		OSC_DELAY(1);
	}
	if(ty == 0) OSC.CHTrig = OSCTrig_Nnoe;
	for(i = 0; i < 5; i++) {
		MCURead_H; Delay(100);
		MCURead_L; Delay(100);
	}
	
	switch(OSC.CHVoltageSel[0]) {
		case OSCVoltage_10V: CH1Rate = 0.74; break;
		case OSCVoltage_5V:  CH1Rate = 0.72; break;
		case OSCVoltage_2V:  CH1Rate = 0.85; break;
		case OSCVoltage_1V:  CH1Rate = 0.83; break;
	}
	
	switch(OSC.CHVoltageSel[1]) {
		case OSCVoltage_10V: CH2Rate = 0.74; break;
		case OSCVoltage_5V:  CH2Rate = 0.72; break;
		case OSCVoltage_2V:  CH2Rate = 0.85; break;
		case OSCVoltage_1V:  CH2Rate = 0.83; break;
	}
	
	for(i = 0; i < 1024; i++) {
		MCURead_H;
		Delay(100);
		MCURead_L;
		Delay(100);
//		*Buf1++ = (INT8S)( (float)((INT8S)(SRAMData & 0xff) - CH1Base) * CH1Rate );
//		*Buf2++ = (INT8S)( (float)((INT8S)((SRAMData >> 8) & 0xff) - CH2Base) * CH2Rate );
		
		*Buf1 = (INT8S)(SRAMData & 0xff) - CH1Base;
		*Buf2 = (INT8S)((SRAMData >> 8) & 0xff) - CH2Base;
		*Buf1++ *= CH1Rate;
		*Buf2++ *= CH2Rate;

//		*Buf1++ = (INT8S)(SRAMData & 0xff) - CH1Base;
//		*Buf2++ = (INT8S)((SRAMData >> 8) & 0xff) - CH2Base;
	
	}
}

/*---------------------------------------------------------------------------------
								获取频率
---------------------------------------------------------------------------------*/
void OSC_GetFrequency(INT16U *Buf)
{
	INT8U i;
	for(i = 0; i < 6; i++) {
		MCUReadFre_H;
		Delay(0xff);
		printf("%4x ", SRAMData);
		MCUReadFre_L;
		*Buf++ = SRAMData;
	}
	printf("\r\n");
}

/*---------------------------------------------------------------------------------
								排序（小到大）
---------------------------------------------------------------------------------*/
void Sort_SmallToLarge(INT8U *p1, INT8U *p2)
{
	INT8S tmp;
	
	if(*p1 > *p2) {
		tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
	} 
}

/*---------------------------------------------------------------------------------
								绘波形
---------------------------------------------------------------------------------*/
#define OSC_DrawBaseAdd 40   /* 起始Y地址 */
void OSC_Draw(INT8S *Buf1, INT8U CH1En, INT8S *Buf2, INT8U CH2En, INT8S *Buf3, INT8U MathEn)
{
	INT16U i, j, Drawed, Color;
	INT8U Point1[2], Point2[2], Point3[2], VF;
	
	for(i = 0; i < 479; i++) {
		AddressSet(OSC_DrawBaseAdd, 479 - i, OSC_DrawBaseAdd + 179, 479 - i);
		VF = 0;
		if((i + 1) % 50 == 0) VF = 1; /* 画竖线 */

		for(j = 0; j < 180; j++) {
			Drawed = 0;
			Color  = MyLCDBlack;
			
			Point1[0] = *(Buf1 + i) + 90; Point1[1] = *(Buf1 + i + 1) + 90; 
			Point2[0] = *(Buf2 + i) + 90; Point2[1] = *(Buf2 + i + 1) + 90;
			Point3[0] = *(Buf3 + i) + 90; Point3[1] = *(Buf3 + i + 1) + 90;
			
			Sort_SmallToLarge(&Point1[0], &Point1[1]);
			Sort_SmallToLarge(&Point2[0], &Point2[1]);
			Sort_SmallToLarge(&Point3[0], &Point3[1]);
			
			if(j >= Point1[0] && j <= Point1[1] && CH1En && !Drawed) {
				Color = MyLCDRed;
				Drawed = 1;
			}
			if(j >= Point2[0] && j <= Point2[1] && CH2En && !Drawed) {
				Color = MyLCDYellow;
				Drawed = 1;
			}
			if(j >= Point2[0] && j <= Point2[1] && CH2En && !Drawed) {
				Color = MyLCDGreen;
				Drawed = 1;
			}
			if(VF == 1) {
				if((j & 0x0003) == 0x0000) {
					Color = RGB2BIT16(15, 15, 15);
				}
			}
			else if((i & 0x0003) == 0x0000 && (j + 1) % 30 == 0){
				Color = RGB2BIT16(15, 15, 15);
			}
			*(__IO uint16_t *) ((uint32_t)0x60020000) = Color;
		}
		
		if(i % 50 == 0 && TouchDown) {
			GUI_Exec();
			GUI_TOUCH_Exec();
		}
	}
}

/*---------------------------------------------------------------------------------
								枚举转字符串
---------------------------------------------------------------------------------*/
char *Enum2String(INT8U Type, /* type: 0-时间， 1-电压， 2-触发 */
				  INT8U tmp)
{
	
	if(Type == 0) {
		switch(tmp) {
			case 0:  return "1US";
			case 1:  return "2US";
			case 2:  return "5US";
			case 3:  return "10US";
			case 4:  return "20US";
			case 5:  return "50US";
			case 6:  return "100US";
			case 7:  return "200US";
			case 8:  return "500US";
			
			case 9:  return "1MS";
			case 10: return "2MS";
			case 11: return "5MS";
			case 12: return "10MS";
			case 13: return "20MS";
			case 14: return "50MS";
			case 15: return "100MS";
			case 16: return "200MS";
			case 17: return "500MS";
			default: break;
		}
	}
	else if(Type == 1) {
		switch(tmp) {
			case 0:  return "10V";
			case 1:  return "5V";
			case 2:  return "2V";
			case 3:  return "1V";
			case 4:  return "500MV";
			case 5:  return "200MV";
			case 6:  return "100MV";
			case 7:  return "50MV";
			default: break;
		}
	}
	else {
		switch(tmp) {
			case 0:  return "None";
			case 1:  return "1-Fal";
			case 2:  return "1-Ris";
			case 3:  return "2-Fal";
			case 4:  return "2-Ris";
			default: break;
		}
	}
	
	return " ";
}

