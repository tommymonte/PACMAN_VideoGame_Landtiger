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

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
#define UP 1
#define DOWN 2
#define DX 3
#define SX 4
#define NOP 0

extern int direction;


void RIT_IRQHandler (void) {
		
    static int up = 0, down = 0, sx = 0, dx = 0;

    // Controllo joystick UP
    if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) {	
        up++;
        if (up == 1) {  // Solo al primo rilevamento
            direction = UP;  // Aggiorna la direzione
        }
    } else {
        up = 0;  // Resetta il contatore
    }

    // Controllo joystick DOWN
    if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) {	
        down++;
        if (down == 1) {  // Solo al primo rilevamento
            direction = DOWN;
        }
    } else {
        down = 0;
    }

    // Controllo joystick LEFT
    if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) {	
        sx++;
        if (sx == 1) {  // Solo al primo rilevamento
            direction = SX;
        }
    } else {
        sx = 0;
    }

    // Controllo joystick RIGHT
    if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) {	
        dx++;
        if (dx == 1) {  // Solo al primo rilevamento
            direction = DX;
        }
    } else {
        dx = 0;
    }

    // Cancella il flag di interrupt
    LPC_RIT->RICTRL |= 0x1;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
