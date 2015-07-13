#include "main.h"
#include "GUI.h"
#include "OSC.h"
#include "DIALOG.h"

static INT8S OSC_CH1Buf[1026];
static INT8S OSC_CH2Buf[1026];
static INT16U OSC_FreqBuf[6];


extern OSC_Struct OSC;
/*---------------------------------------------------------------------------------
								OSC main
---------------------------------------------------------------------------------*/
void Task1(void *p)
{
	WM_HWIN WindowUp;
	WM_HWIN WindowBottom;
    
    GUI_Init();	
	GUI_CURSOR_Show();
	OSC_StructInit();
	WindowBottom = CreateWindow();
	WindowUp = CreateFramewin();
	
	while(1) {
		OSC_GetADData(OSC_CH1Buf, OSC_CH2Buf);
		OSC_Draw(OSC_CH1Buf, OSC.CHEnable[0], OSC_CH2Buf, OSC.CHEnable[1], OSC_CH1Buf, OSC.CHEnable[2]);
		GUI_Delay(10);
	}
}

/*---------------------------------------------------------------------------------
								  LED
---------------------------------------------------------------------------------*/
void Task2(void *p) 
{
	p = p;
	
	while(1) {
	    OSTimeDlyHMSM(0, 0, 0, 500);
		SetLED2;
		OSTimeDlyHMSM(0, 0, 0, 500);
		ClrLED2;
	}
}

/*---------------------------------------------------------------------------------
								  TEST
---------------------------------------------------------------------------------*/
void TaskTest(void *p) 
{
	p = p;
	
	while(1) {
	    OSTimeDlyHMSM(0, 0, 0, 10);
		SetLED1;
		OSTimeDlyHMSM(0, 0, 0, 10);
		ClrLED1;
	}
}

/*---------------------------------------------------------------------------------
								 Touch
---------------------------------------------------------------------------------*/
void Task3(void *p)
{
	p = p;
	
	while(1) {
		OSTimeDlyHMSM(0, 0, 0, 10);
		GUI_TOUCH_Exec();
	}
}

/*---------------------------------------------------------------------------------
								
---------------------------------------------------------------------------------*/
static OS_STK Task1_STK[512];  
#define Task1_PRIO   5  

static OS_STK Task2_STK[512]; 
#define Task2_PRIO   6

static OS_STK Task3_STK[512]; 
#define Task3_PRIO   7

static OS_STK TaskTest_STK[512]; 
#define TaskTest_PRIO   8

void CreateTask(void *p)
{
	p = p;
	OSTaskCreate(Task2,(void *)0, &Task2_STK[512 - 1], Task2_PRIO);
	OSTaskCreate(Task1,(void *)0, &Task1_STK[512 - 1], Task1_PRIO);
	OSTaskCreate(Task3,(void *)0, &Task3_STK[512 - 1], Task3_PRIO);
	OSTaskCreate(TaskTest,(void *)0, &TaskTest_STK[512 - 1], TaskTest_PRIO);
	OSTaskDel(OS_PRIO_SELF);
}


