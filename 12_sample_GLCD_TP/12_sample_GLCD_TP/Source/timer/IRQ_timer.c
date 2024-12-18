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

void TIMER0_IRQHandler(void) {
    static int cnt = 60;               // Mantiene il conto del tempo di gioco
    char buffer[10];                   // Buffer per il testo da stampare
		static int delayCounter = 0;         // Contatore per il ritardo casuale
    static int randomDelay = 5;          // Valore del ritardo casuale
		static int totalPillsPlaced = 0;

    // Se pausa è attiva, mostra "PAUSE", altrimenti mostra il contatore
    if (pause == 0) {
				GUI_Text(0, 0, (uint8_t*)"CCCCCCCCC", Black, Black);  // Visualizza "PAUSE"
        sprintf(buffer, "PAUSE");
        GUI_Text(0, 0, (uint8_t*)buffer, Red, Black);  // Visualizza "PAUSE"
		} else if (pause == 2) {
				sprintf(buffer, "GAME OVER");
        GUI_Text(0, 0, (uint8_t*)buffer, Red, Black);  // Visualizza "PAUSE"
		} else if (pause == 3){
			sprintf(buffer, "VICTORY!");
        GUI_Text(0, 0, (uint8_t*)buffer, Green, Black);  // Visualizza "PAUSE"
		} else {
        sprintf(buffer, "Time: %d", cnt);
        GUI_Text(0, 0, (uint8_t*)buffer, Red, Black);  // Visualizza il tempo
    }

    if (LPC_TIM0->IR & 1 && pause == 1) {  // Verifica se l'interruzione è causata dal Timer 0
        if (cnt == 0 && countPills(screen) > 0) {
							// game over
							pause = 2;
				}	else {
							cnt--;  // Decrementa il contatore
        }
				
				// Se non ho ancora posizionato 6 pills
        if (totalPillsPlaced < 6) {
					if (randomDelay == 0){
						delayCounter = 0;
						randomDelay = (5 + (score % 5));
						distributePills(score, screen);
						totalPillsPlaced++;
					} else {
						randomDelay--;
					}
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
int score = 0;
int pause = 0;
int lives = 1;
int cnt_score = 0;

void TIMER1_IRQHandler(void) {
    if (LPC_TIM1->IR & 1) {  // Verifica dell'interrupt
			                     // Controllo per la raccolta delle pills
			if (pause == 1) {
				if (screen[pacman.y][pacman.x] == 2) {  
                        screen[pacman.y][pacman.x] = 0;  // Aggiorna la matrice
												drawIcon(pacman.x *10, pacman.y * 10, pill, Black);
                        score += 10; // Aumenta il punteggio
												cnt_score += 10;
        } else if (screen[pacman.y][pacman.x] == 3) {  
                        // Pac-Man raccoglie una pill speciale
                        screen[pacman.y][pacman.x] = 0;  // Aggiorna la matrice
												drawIcon(pacman.x*10, pacman.y*10, powerPill, Black);
                        score += 50; // Aumenta il punteggio speciale
												cnt_score += 50;
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
				printScore(score);
				
        if (cnt_score == 1000) {
					lives++;
					cnt_score = 0;
				}
			} 
				
			if (countPills(screen) == 0){
					pause = 3;
				
			}
				
				
			//life(lives);
				
			printLife(lives);
				
			  moveGhost(&ghost, &pacman, screen);
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
	  
		if ((LPC_TIM2->IR & 1)) {  // Verifica dell'interrupt
				
				LPC_TIM2->IR = 1;
		}
 }
/******************************************************************************
**                            End Of File
******************************************************************************/
