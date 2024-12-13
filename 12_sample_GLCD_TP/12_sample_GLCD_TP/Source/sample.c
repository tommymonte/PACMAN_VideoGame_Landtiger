/****************************************Copyright (c)****************************************************
**--------------------------------------------------------------------------------------------------------
** Modified by:             Tommaso Montedoro
** Version:                 v1.0
** Descriptions:            first Assignment
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "GameFeatures/matrixGame.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "GLOBALS.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

int direction  = NOP; // default


int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  init_RIT(0x4C4B40);
	enable_RIT();
	LCD_Initialization();
	joystick_init();
	LCD_Clear(Black);
	

	drawScreenFromMatrix(screen);
	//init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = GUI_Text(0, 0, (uint8_t*)buffer, Red, Black); */
	//init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	init_timer(1, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	init_timer(0, 0x1312D0 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */
	
	enable_timer(0);
	enable_timer(1);
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
