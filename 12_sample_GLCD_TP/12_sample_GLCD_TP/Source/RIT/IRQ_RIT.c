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
    // 1
    {c3, time_semicroma},    // Do
    {c4, time_semicroma},    // Re
    {g3, time_semicroma},    // Mi
    {e3, time_semicroma},    // Sol 
    {c4, time_croma},    // Sol
    {e3, time_croma},// Mi
    {e3b, time_semicroma},    // Do
    {e4b, time_semicroma},    // Sol basso
    {b3b, time_semicroma},// La
    {f3, time_semicroma},// Si
		{pause, time_semicroma},    
		{d4, time_croma},    // Do 
    {a3, time_croma},    // Do
    {f3, time_croma},// Si
    // 1
    {c3, time_semicroma},    // Do
    {c4, time_semicroma},    // Re
    {g3, time_semicroma},    // Mi
    {e3, time_semicroma},    // Sol 
    {c4, time_croma},    // Sol
    {e3, time_croma},// Mi
    {e3b, time_semicroma},    // Do
    {e4b, time_semicroma},    // Sol basso
    {b3b, time_semicroma},// La
    {f3, time_semicroma},// Si
		{pause, time_semicroma},    
		{d4, time_croma},    // Do 
    {a3, time_croma},    // Do
    {f3, time_croma},// Si
    // 1
    {c3, time_semicroma},    // Do
    {c4, time_semicroma},    // Re
    {g3, time_semicroma},    // Mi
    {e3, time_semicroma},    // Sol 
    {c4, time_croma},    // Sol
    {e3, time_croma},// Mi
    {e3b, time_semicroma},    // Do
    {e4b, time_semicroma},    // Sol basso
    {b3b, time_semicroma},// La
    {f3, time_semicroma},// Si
		{pause, time_semicroma},    
		{d4, time_semiminima},    // Do 
    {a3, time_semiminima},    // Do
    {f3, time_croma},// Si
		// 4
		{c3, time_semicroma},    // Do
    {c4, time_semicroma},    // Re
    {g3, time_semicroma},    // Mi
    {e3, time_semicroma},    // Sol 
    {c4, time_croma},    // Sol
		{g3, time_croma},
		{e3, time_croma},
		{e3b, time_semicroma},
		{f3, time_semicroma},
		{f3, time_semicroma},
		{g3, time_semicroma},
		{g3, time_semicroma},
		{a3, time_semicroma},
		{c4, time_semicroma},
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
