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
#include "../led/led.h"
#include "../timer/timer.h"
/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
/*
extern int direction;
volatile int press=0;
volatile int press_1=0;
volatile int press_2=0;
extern int flag_music;
*/
// beat 1/4 = 1.65/4 seconds
#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32
#define UPTICKS 1

//SHORTENING UNDERTALE: TOO MANY REPETITIONS
NOTE song[] = 
{
    // 1° parte
    {c4, time_semicroma},    // Do (ottava 4)
    {c5, time_semicroma},    // Do (ottava 5)
    {g4, time_semicroma},    // Sol
    {e4, time_semicroma},    // Mi
    {c5, time_croma},        // Do alto
    {e4, time_croma},        // Mi
    {e4b, time_semicroma},   // Mi bemolle
    {e5b, time_semicroma},   // Mi bemolle (ottava 5)
    {b4b, time_semicroma},   // Si bemolle
    {f4, time_semicroma},    // Fa
    {d5, time_semiminima},   // Re alto
    {a4, time_semiminima},   // La
    {f4, time_croma},        // Fa
    
    // 2° parte (ripetizione)
    {c4, time_semicroma},    // Do
    {c5, time_semicroma},    // Do (ottava 5)
    {g4, time_semicroma},    // Sol
    {e4, time_semicroma},    // Mi
    {c5, time_croma},        // Do alto
    {e4, time_croma},        // Mi
    {e4b, time_semicroma},   // Mi bemolle
    {e5b, time_semicroma},   // Mi bemolle (ottava 5)
    {b4b, time_semicroma},   // Si bemolle
    {f4, time_semicroma},    // Fa
    {d5, time_semiminima},   // Re alto
    {a4, time_semiminima},   // La
    {f4, time_croma},        // Fa

    // 3° parte
    {c4, time_semicroma},    // Do
    {c5, time_semicroma},    // Do (ottava 5)
    {g4, time_semicroma},    // Sol
    {e4, time_semicroma},    // Mi
    {c5, time_croma},        // Do alto
    {g4, time_croma},        // Sol
    {e4, time_croma},        // Mi
    {e4b, time_semicroma},   // Mi bemolle
    {f4, time_semicroma},    // Fa
    {f4, time_semicroma},    // Fa
    {g4, time_semicroma},    // Sol
    {g4, time_semicroma},    // Sol
    {a4, time_semicroma},    // La
    {c5, time_semicroma},    // Do alto
};

void RIT_IRQHandler (void)
{
	static int currentNote = 0;
	static int ticks = 0;
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
		enable_timer(0);
		enable_timer(1);
	}
	
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}
/******************************************************************************
**                            End Of File
******************************************************************************/
