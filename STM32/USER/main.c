#include "main.h"

static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];	

void GUIDEMO_main(void);
  
int main(void)
{
  	BSP_Init();
	
	OSInit();
	OSTaskCreate(CreateTask,(void *)0, &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO);

	OSStart();
    return 0;
 }


