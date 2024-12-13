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
    
		// Verifica se l'interruzione è causata dal Timer 0
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


typedef struct {
    int x; // Posizione X
    int y; // Posizione Y
} PacMan;
PacMan pacman = {0, 150}; // Posizione iniziale di Pac-Ma

volatile int pacman_direction = 0; // Direzione di Pac-Man
// Funzione per disegnare un quadrato 5x5 usando linee

void drawSquares(int x, int y, uint16_t color) {
    // Disegna le 4 linee del quadrato
    LCD_DrawLine(x, y, x + 14, y, color);       // Linea superiore (orizzontale)
    LCD_DrawLine(x, y, x, y + 14, color);       // Linea sinistra (verticale)
    LCD_DrawLine(x, y + 14, x + 14, y + 14, color); // Linea inferiore (orizzontale)
    LCD_DrawLine(x + 14, y, x + 14, y + 14, color); // Linea destra (verticale)
}



void TIMER1_IRQHandler (void)
{
 if (LPC_TIM1->IR & 1) {  // Verifica interrupt del Timer 1
        // Cancella la posizione precedente di Pac-Man
        drawSquares(pacman.x, pacman.y, Black);

        // Aggiorna la posizione di Pac-Man
        switch (direction) {
            case UP:
                pacman.y -= 5;
                if (pacman.y < 7) pacman.y = 7;  // Limite superiore
                break;
            case DOWN:
                pacman.y += 5;
                if (pacman.y > 233) pacman.y = 233;  // Limite inferiore
                break;
            case SX:
                pacman.x -= 5;
                if (pacman.x < 7) pacman.x = 7;  // Limite sinistro
                break;
            case DX:
                pacman.x += 5;
                if (pacman.x > 313) pacman.x = 313;  // Limite destro
                break;
        }

        // Disegna Pac-Man nella nuova posizione
        drawSquares(pacman.x, pacman.y, Yellow);

        // Cancella il flag di interrupt del Timer
        LPC_TIM1->IR = 1;
    }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
