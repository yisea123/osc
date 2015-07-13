/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.12                          *
*        Compiled Jun 29 2011, 15:28:47                              *
*        (c) 2011 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"

#include "OSC.h"
extern OSC_Struct OSC;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define ID_WINDOW_0       (GUI_ID_USER + 0x00)

#define ID_TEXT_Time      (GUI_ID_USER + 0x01)
#define ID_SLIDER_Time    (GUI_ID_USER + 0x02)
#define ID_EDIT_Time      (GUI_ID_USER + 0x03)

#define ID_TEXT_CH1Vol    (GUI_ID_USER + 0x04)
#define ID_SLIDER_CH1Vol  (GUI_ID_USER + 0x05)
#define ID_EDIT_CH1Vol    (GUI_ID_USER + 0x06)

#define ID_TEXT_CH2Vol    (GUI_ID_USER + 0x07)
#define ID_SLIDER_CH2Vol  (GUI_ID_USER + 0x08)
#define ID_EDIT_CH2Vol    (GUI_ID_USER + 0x09)

#define ID_TEXT_Trig      (GUI_ID_USER + 0x0A)
#define ID_SLIDER_Trig    (GUI_ID_USER + 0x0B)
#define ID_EDIT_Trig      (GUI_ID_USER + 0x0C)

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 	   0,   219, 480, 100, 0, 0   },
 
  { TEXT_CreateIndirect,   "Time",   ID_TEXT_Time,     2,   3,   26,  16,  0, 0   },
  { SLIDER_CreateIndirect, "Slider", ID_SLIDER_Time,   29,  3,   80,  20,  0, 0   }, 
  { EDIT_CreateIndirect,   "Edit",   ID_EDIT_Time,     116, 3,   46,  20,  0, 0   },
 
  { TEXT_CreateIndirect,   "Vol1",    ID_TEXT_CH1Vol,   3,   27,  26,  17,  0, 0   },
  { SLIDER_CreateIndirect, "Slider", ID_SLIDER_CH1Vol, 29,  27,  80,  20,  0, 0   },
  { EDIT_CreateIndirect,   "Edit",   ID_EDIT_CH1Vol,   116, 27,  46,  20,  0, 0   },
  
  { TEXT_CreateIndirect,   "Vol2",    ID_TEXT_CH2Vol,   3,   51,  26,  17,  0, 0   },
  { SLIDER_CreateIndirect, "Slider", ID_SLIDER_CH2Vol, 29,  51,  80,  20,  0, 0   },
  { EDIT_CreateIndirect,   "Edit",   ID_EDIT_CH2Vol,   116, 51,  46,  20,  0, 0   },
  
  { TEXT_CreateIndirect,   "Trig",    ID_TEXT_Trig,     3,   75,  26,  17,  0, 0   },
  { SLIDER_CreateIndirect, "Slider", ID_SLIDER_Trig,   29,  75,  80,  20,  0, 0   },
  { EDIT_CreateIndirect,   "Edit",   ID_EDIT_Trig,     116, 75,  46,  20,  0, 0   },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int Id, NCode;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_Time);
		SLIDER_SetRange(hItem, 0, 14);
		SLIDER_SetValue(hItem, OSC.CHTimeSel); 
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Time);
		EDIT_SetText(hItem, Enum2String(0, OSC.CHTimeSel));

		hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_CH1Vol);
		SLIDER_SetRange(hItem, 0, 3);
		SLIDER_SetValue(hItem, OSC.CHVoltageSel[0]); 
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CH1Vol);
		EDIT_SetText(hItem, Enum2String(1, OSC.CHVoltageSel[0]));

		hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_CH2Vol);
		SLIDER_SetRange(hItem, 0, 3);
		SLIDER_SetValue(hItem, OSC.CHVoltageSel[1]);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CH2Vol);
		EDIT_SetText(hItem, Enum2String(1, OSC.CHVoltageSel[1]));

		hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_Trig);
		SLIDER_SetRange(hItem, 0, 4);
		SLIDER_SetValue(hItem, OSC.CHTrig);
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Trig);
		EDIT_SetText(hItem, Enum2String(2, OSC.CHTrig));
	break;

	case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
		
			case ID_SLIDER_Time: // Notifications sent by 'Slider'
				switch(NCode) {
					case WM_NOTIFICATION_CLICKED:

					break;
					case WM_NOTIFICATION_RELEASED:

					break;
					case WM_NOTIFICATION_VALUE_CHANGED:
						hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_Time);
						OSC.CHTimeSel = SLIDER_GetValue(hItem);	

						hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Time);
						EDIT_SetText(hItem, Enum2String(0, OSC.CHTimeSel));
					break;
				}
			break;
			
			case ID_SLIDER_CH1Vol: // Notifications sent by 'Slider'
				switch(NCode) {
					case WM_NOTIFICATION_CLICKED:

					break;
					case WM_NOTIFICATION_RELEASED:

					break;
					case WM_NOTIFICATION_VALUE_CHANGED:
						hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_CH1Vol);
						OSC.CHVoltageSel[0] = SLIDER_GetValue(hItem);
						
						hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CH1Vol);
						EDIT_SetText(hItem, Enum2String(1, OSC.CHVoltageSel[0]));
					break;
				}
			break;

			case ID_SLIDER_CH2Vol: // Notifications sent by 'Slider'
				switch(NCode) {
					case WM_NOTIFICATION_CLICKED:

					break;
					case WM_NOTIFICATION_RELEASED:

					break;
					case WM_NOTIFICATION_VALUE_CHANGED:
						hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_CH2Vol);
						OSC.CHVoltageSel[1] = SLIDER_GetValue(hItem);
					
						hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CH2Vol);
						EDIT_SetText(hItem, Enum2String(1, OSC.CHVoltageSel[1]));
					break;
				}
			break;

			case ID_SLIDER_Trig: // Notifications sent by 'Slider'
				switch(NCode) {
					case WM_NOTIFICATION_CLICKED:

					break;
					case WM_NOTIFICATION_RELEASED:

					break;
					case WM_NOTIFICATION_VALUE_CHANGED:
						hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_Trig);
						OSC.CHTrig = SLIDER_GetValue(hItem);
						
						hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_Trig);
						EDIT_SetText(hItem, Enum2String(2, OSC.CHTrig));
					break;
				}
			break;
		  
			case ID_EDIT_Time: // Notifications sent by 'Edit'
				switch(NCode) {
					case WM_NOTIFICATION_CLICKED:

					break;
					case WM_NOTIFICATION_RELEASED:

					break;
					case WM_NOTIFICATION_VALUE_CHANGED:

					break;
				}
			break;

			case ID_EDIT_CH1Vol: // Notifications sent by 'Edit'
				switch(NCode) {
					case WM_NOTIFICATION_CLICKED:

					break;
					case WM_NOTIFICATION_RELEASED:

					break;
					case WM_NOTIFICATION_VALUE_CHANGED:

					break;
				}
			break;
		}
		break;

		default:
			WM_DefaultProc(pMsg);
		break;
	}
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN CreateWindow(void);
WM_HWIN CreateWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/