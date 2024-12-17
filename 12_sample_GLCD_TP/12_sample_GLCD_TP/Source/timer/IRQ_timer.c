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
int totalPillsPlaced = 6;   // Contatore globale delle pills posizionate
extern uint8_t lfsr_register;  // Registro LFSR iniziale

void TIMER0_IRQHandler(void) {
    static int cnt = 60;                 // Mantiene il conto del tempo di gioco

    char buffer[10];                     // Buffer per il testo da stampare

    // Aggiorna il display con il valore del contatore
    sprintf(buffer, "Time: %d", cnt);
    GUI_Text(0, 0, (uint8_t*)buffer, Red, Black);

    if (LPC_TIM0->IR & 1) {  // Verifica se l'interruzione è causata dal Timer 0
        if (cnt == 0) {
            cnt = 60;  // Resetta il contatore del tempo
        } else {
            cnt--;  // Decrementa il contatore
        }

    // Ripulisce il flag di interruzione del Timer 0
    LPC_TIM0->IR = 1;
    return;
}
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
int score = 0;

void TIMER1_IRQHandler(void) {
    if (LPC_TIM1->IR & 1) {  // Verifica dell'interrupt
			                     // Controllo per la raccolta delle pills
				if (screen[pacman.y][pacman.x] == 2) {  
                        screen[pacman.y][pacman.x] = 0;  // Aggiorna la matrice
												drawIcon(pacman.x *10, pacman.y * 10, pill, Black);
                        score += 10; // Aumenta il punteggio
        } else if (screen[pacman.y][pacman.x] == 3) {  
                        // Pac-Man raccoglie una pill speciale
                        screen[pacman.y][pacman.x] = 0;  // Aggiorna la matrice
												drawIcon(pacman.x*10, pacman.y*10, powerPill, Black);
                        score += 50; // Aumenta il punteggio speciale
                        //lives++;     // Aumenta le vite di Pac-Man
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
                if (pacman.y < 29 && (screen[pacman.y + 1][pacman.x] == 0 || screen[pacman.y + 1][pacman.x] == 2 || screen[pacman.y + 1][pacman.x] == 3)) {
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
                } else if (pacman.x == 23 && pacman.y == 16) {
                  pacman.x = 0;
                }
                break;
        }
				drawPacman();
				// int cnt_pills = countPills(screen);
				printScore(score);
        // Cancella il flag di interruzione del Timer
        LPC_TIM1->IR = 1;
    }
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER2_IRQHandler(void) {
	   static int delayCounter = 0;         // Contatore per il ritardo casuale
    static int randomDelay = 0;          // Valore del ritardo casuale
	 if (LPC_TIM2->IR & 1) {  // Verifica dell'interrupt

	// Se non ho ancora posizionato 6 pills
        if (totalPillsPlaced < 6) {
            // Controlla se il ritardo è completato
            if (delayCounter >= randomDelay) {
                // Posiziona una pillola
                distributePills(lfsr_register, screen);
                totalPillsPlaced++;

                // Genera un nuovo ritardo casuale
                randomDelay = (lfsr(lfsr_register) & 0x1F) + 10;  // Valore tra 10 e 41
                delayCounter = 0;  // Resetta il contatore
            } else {
                delayCounter++;  // Incrementa il contatore di ritardo
            }
        }
		 			LPC_TIM2->IR = 1;
	 }
}


/******************************************************************************
**                            End Of File
******************************************************************************/
