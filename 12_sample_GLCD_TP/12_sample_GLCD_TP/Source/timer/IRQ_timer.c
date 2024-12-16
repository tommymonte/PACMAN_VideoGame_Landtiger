/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../joystick/joystick.h"
#include "../RIT/RIT.h"
#include "GLOBALS.h"
#include <stdio.h> /*for sprintf*/

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


void TIMER0_IRQHandler (void)
{
    static int cnt = 60;               // Mantiene lo stato tra chiamate
    char buffer[10];                   // Buffer per il testo da stampare
	
    // Aggiorna il display con il valore del contatore
    sprintf(buffer, "Time: %d", cnt);
    GUI_Text(0, 0, (uint8_t*)buffer, Red, Black);
    
		// Verifica se l'interruzione � causata dal Timer 0
    if (LPC_TIM0->IR & 1) {
        // Decrementa il contatore o lo resetta
        if (cnt == 0) {
            cnt = 60;
        } else {
            cnt--;
        }
    }
    // Ripulisce il flag di interruzione del Timer 0
    LPC_TIM0->IR = 1;
    return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/




volatile int pacman_direction = 0; // Direzione di Pac-Man
// Funzione per disegnare un quadrato 5x5 usando linee


void TIMER1_IRQHandler(void) {
    if (LPC_TIM1->IR & 1) {  // Verifica dell'interrupt
			                     // Controllo per la raccolta delle pills
				if (screen[pacman.y][pacman.x] == 2) {  
                        screen[pacman.y][pacman.x] = 0;  // Aggiorna la matrice
												drawIcon(pacman.x *10, pacman.y * 10, pill, Black);
                       // score += 10; // Aumenta il punteggio
        } else if (screen[pacman.y][pacman.x] == 3) {  
                        // Pac-Man raccoglie una pill speciale
                        screen[pacman.y][pacman.x] = 0;  // Aggiorna la matrice
												drawIcon(pacman.x*10, pacman.y*10, powerPill, Black);
                       // score += 50; // Aumenta il punteggio speciale
                       // lives++;     // Aumenta le vite di Pac-Man
        }
        // Aggiorna la posizione di Pac-Man in base alla direzione	
        switch (direction) {
            case UP:
                if (pacman.y > 0 && (screen[pacman.y - 1][pacman.x] == 0 || screen[pacman.y - 1][pacman.x] == 2 || screen[pacman.y - 1][pacman.x] == 3)) {
										prevPacman = currentPacman;	
										currentPacman = pacMan_up;
                    pacman.y--;
                }
                break;
            case DOWN:
                if (pacman.y < 24 && (screen[pacman.y + 1][pacman.x] == 0 || screen[pacman.y + 1][pacman.x] == 2 || screen[pacman.y + 1][pacman.x] == 3)) {
                    prevPacman = currentPacman;	
					          currentPacman = pacMan_down;    
								    pacman.y++;
                }
                break;
            case SX:
                if (pacman.x > 0 && (screen[pacman.y][pacman.x - 1] == 0 || screen[pacman.y][pacman.x - 1] == 2 || screen[pacman.y][pacman.x - 1] == 3)) {
                    prevPacman = currentPacman;	
					          currentPacman = pacMan_sx;    
								    pacman.x--;
                } else if (pacman.x == 0 && pacman.y == 16) {
                    pacman.x = 23;
                }
                break;
            case DX:
                if (pacman.x < 23 && (screen[pacman.y][pacman.x + 1] == 0  || screen[pacman.y][pacman.x + 1] == 2 || screen[pacman.y][pacman.x + 1] == 3)) {
                    prevPacman = currentPacman;	
					          currentPacman = pacMan;    
								    pacman.x++;
                } else if (pacman.x = 23 && pacman.y ==16) {
                  pacman.x = 0;
                }
                break;
        }

				drawPacman();
        // Cancella il flag di interruzione del Timer
        LPC_TIM1->IR = 1;
    }
}

/*
void TIMER2_IRQHandler(void) {
	 if (LPC_TIM2->IR & 1) {  // Verifica dell'interrupt
			LPC_TIM2->IR = 1;
		 drawPacman();
		 
	 }
}
*/

/******************************************************************************
**                            End Of File
******************************************************************************/
