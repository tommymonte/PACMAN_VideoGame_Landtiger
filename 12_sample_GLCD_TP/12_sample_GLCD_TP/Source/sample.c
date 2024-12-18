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

PacMan pacman = {12, 18}; // Posizione iniziale di Pac-Man
int (*currentPacman)[10] = pacMan; // Inizializzato alla matrice per "destra"
int (*prevPacman)[10] = pacMan;
uint8_t lfsr_register = 0x01;

int main(void)
{
SystemInit();  												/* System Initialization (i.e., PLL)  */
  
	BUTTON_init();
	init_RIT(0x004C4B40);
	LCD_Initialization();
	joystick_init();
	LCD_Clear(Black);
	
	enable_RIT();
	
	//distributePills(lfsr_register, screen);
	drawScreenFromMatrix(screen, pill, powerPill);
	
	init_timer(1, 0x2625A0); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	init_timer(0, 0x17D7840 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */
	init_timer(2, 25000000); 						    /* 8us * 25MHz = 200 ~= 0xC8 */
	
	enable_timer(0);
	enable_timer(1);
	enable_timer(2);

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
