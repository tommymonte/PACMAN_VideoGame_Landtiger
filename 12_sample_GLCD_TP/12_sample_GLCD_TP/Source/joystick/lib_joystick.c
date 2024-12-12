/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {
	/* Joystick su (P1.29) */
    LPC_PINCON->PINSEL3 &= ~(3<<26);  // Configurazione GPIO per P1.29
    LPC_GPIO1->FIODIR   &= ~(1<<29);  // Configurato come input
	
	  LPC_PINCON->PINSEL3 &= ~(3<<24);  // Configurazione GPIO per P1.29
    LPC_GPIO1->FIODIR   &= ~(1<<28);  // Configurato come input
	
	  LPC_PINCON->PINSEL3 &= ~(3<<22);  // Configurazione GPIO per P1.29
    LPC_GPIO1->FIODIR   &= ~(1<<27);  // Configurato come input
	
	  LPC_PINCON->PINSEL3 &= ~(3<<20);  // Configurazione GPIO per P1.29
    LPC_GPIO1->FIODIR   &= ~(1<<26);  // Configurato come input
}
