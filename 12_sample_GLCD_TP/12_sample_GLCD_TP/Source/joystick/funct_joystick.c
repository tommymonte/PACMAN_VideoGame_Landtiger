/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        High level joystick management functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "joystick.h"


/*----------------------------------------------------------------------------
  Function that turns on requested led
 *----------------------------------------------------------------------------*/
void joystick_On(unsigned int num) {
}

#define UP 1
#define DOWN 2
#define DX 3
#define SX 4
#define NOP 0

int joystick_read(void) {
	int direction = NOP;
	
  /* UP */
	if((LPC_GPIO1 -> FIOPIN & (1<<29)) == 0){ 
		/* Joystick UP pressed */
		direction = UP;
	}	
	/* DOWN */
	if((LPC_GPIO1 -> FIOPIN & (1<<26)) == 0){ 
		/* Joystick DOWN pressed */
		direction = DOWN;
	}
	/* SX */
	if((LPC_GPIO1 -> FIOPIN & (1<<27)) == 0){ 
		/* Joystick SX pressed */
		direction = SX;
	} // end Joystick (SX)
	/* DX */
	if((LPC_GPIO1 -> FIOPIN & (1<<28)) == 0){ 
		/* Joystick DX pressed */
		direction = DX;
 } // end Joystick (DX)
    return direction; 
}