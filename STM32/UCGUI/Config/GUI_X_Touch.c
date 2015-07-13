/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "GUI_X.h"
#include "stm32f10x.h"
#include "MyLCD.h"

static unsigned int TouchX, TouchY;

void GUI_TOUCH_X_ActivateX(void) {
}

void GUI_TOUCH_X_ActivateY(void) {
}

int  GUI_TOUCH_X_MeasureX(void) {
	if(TouchDown) {
		while(Touch_GetXY(&TouchX, &TouchY) == 0 && TouchDown);
//		printf("x:%4d - y:%4d\r\n", TouchX, TouchY);
		if(TouchDown) {
			if(TouchY > 900) 		TouchX = TouchX * 1765 / 1880;
			else if(TouchY > 600)   TouchX = TouchX * 1765 / (1880 - (1880 - 1857) * (900 - TouchY) / 300);
			else if(TouchY > 400)   TouchX = TouchX * 1765 / (1857 - (1857 - 1758) * (600 - TouchY) / 200);
			return (int)TouchX;
		}
	}
		return 0xffff;
}

int  GUI_TOUCH_X_MeasureY(void) {
	if(TouchDown) {
		while(Touch_GetXY(&TouchX, &TouchY) == 0 && TouchDown);
		return (int)TouchY;
	}
	else 
		return 0xffff;
}


