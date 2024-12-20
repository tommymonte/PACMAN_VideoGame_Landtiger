/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "GLOBALS.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern int direction;
volatile int press=0;
volatile int press_1=0;
volatile int press_2=0;

void RIT_IRQHandler (void) {
    static int up = 0, down = 0, sx = 0, dx = 0;
	
		/*************************INT0***************************/
		if(press !=0){
				press++;
				if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
						switch(press){
								case 2:
										 pause = (pause == 0) ? 1 : 0;  // Se "pause" è 0, lo imposta a 1, altrimenti a 0
										break;
								default:
										break;
						}
				} else {	/* button released */
						press=0;			
						NVIC_EnableIRQ(EINT0_IRQn);							 /* disable Button interrupts			*/
						LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
						}
				} // end INT0
		
		/*********************JOYSTICK**************************/
    // Controllo joystick UP
    if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) {	
        up++;
        if (up == 1) {  // Solo al primo rilevamento
            direction = UP;  // Aggiorna la direzione
						
        }
				down = 0;
				sx = 0;
				dx = 0;
    } 
    // Controllo joystick DOWN
    if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) {	
        down++;
        if (down == 1) {  // Solo al primo rilevamento
            direction = DOWN;
        }
				up = 0;
				sx = 0;
				dx = 0;
    }
    // Controllo joystick LEFT
    if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) {	
        sx++;
        if (sx == 1) {  // Solo al primo rilevamento
            direction = SX;
        }
				dx = 0;
				up = 0;
				down = 0;
    } 
    // Controllo joystick RIGHT
    if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) {	
        dx++;
        if (dx == 1) {  // Solo al primo rilevamento
            direction = DX;
        }
				sx = 0;
				up = 0;
				down = 0;
    }

    // Cancella il flag di interrupt
    LPC_RIT->RICTRL |= 0x1;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
