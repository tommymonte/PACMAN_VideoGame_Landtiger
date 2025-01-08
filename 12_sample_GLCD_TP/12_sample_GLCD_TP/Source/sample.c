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
#include "adc/adc.h"
#include "led/led.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

int direction  = NOP; // default

PacMan pacman = {12, 18}; // Posizione iniziale di Pac-Man
Ghost ghost = {12, 16};  // Posizione iniziale del fantasmino
int (*currentPacman)[10] = pacMan; // Inizializzato alla matrice per "destra"
int (*prevPacman)[10] = pacMan;
uint8_t lfsr_register = 0x01;

int main(void)
{
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	BUTTON_init();
	LED_init();
	
	init_RIT(0x004C4B40);
	
	LCD_Initialization();
	joystick_init();
	LCD_Clear(Black);
	
	drawScreenFromMatrix(screen, pill, powerPill);
	
	init_timer(1, 0x2625A0); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	init_timer(0, 0x17D7840 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */

	enable_RIT();
	ADC_init();
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
		
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
