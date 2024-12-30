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
extern int flag_music;
// beat 1/4 = 1.65/4 seconds
#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32
#define UPTICKS 1

//SHORTENING UNDERTALE: TOO MANY REPETITIONS
NOTE song[] = 
{
   	// 1
	{d3, time_semicroma},
	{d3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 2
	{c3, time_semicroma},
	{c3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 3
	{c3b, time_semicroma},
	{c3b, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 4
	{a2b, time_semicroma},
	{a2b, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 5
};

void RIT_IRQHandler (void) {
    static int up = 0, down = 0, sx = 0, dx = 0;
	
		/*************************INT0***************************/
		if(press !=0){
				press++;
				if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
						switch(press){
								case 2:
										 game_pause = (game_pause == 0) ? 1 : 0;  // Se "pause" è 0, lo imposta a 1, altrimenti a 0
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
		
		/************** MUSIC *****************/
		
		static int currentNote = 0;
		static int ticks = 0;
		
		if ( (LPC_TIM0->TC > 10000)) {
		if(!isNotePlaying())
		{
			++ticks;
			if(ticks == UPTICKS)
			{
				ticks = 0;
				playNote(song[currentNote++]);
			}
		}
	
		if(currentNote == (sizeof(song) / sizeof(song[0])))
		{
			disable_RIT();
		}
	}
		
    // Cancella il flag di interrupt
    LPC_RIT->RICTRL |= 0x1;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
