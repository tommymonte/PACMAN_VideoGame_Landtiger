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
frightened_mode = 0;
static int cnt = 60;               // Mantiene il conto del tempo di gioco
void TIMER0_IRQHandler(void) {
    int spawn_cnt = 10;
    char buffer[10];                   // Buffer per il testo da stampare
		static int delayCounter = 0;         // Contatore per il ritardo casuale
    static int randomDelay = 5;          // Valore del ritardo casuale
		static int totalPillsPlaced = 0;

    // Se pausa è attiva, mostra "PAUSE", altrimenti mostra il contatore
    if (game_pause == 0) {
				GUI_Text(0, 0, (uint8_t*)"CCCCCCCCC", Black, Black);  // Visualizza "PAUSE"
        sprintf(buffer, "PAUSE");
        GUI_Text(0, 0, (uint8_t*)buffer, Red, Black);  // Visualizza "PAUSE"
		} else if (game_pause == 2) {
				sprintf(buffer, "GAME OVER");
        GUI_Text(0, 0, (uint8_t*)buffer, Red, Black);  // Visualizza "PAUSE"
				drawIcon(pacman.x * 10, pacman.y * 10, pacMan, Black);
				disable_timer(1);
		} else if (game_pause == 3){
			sprintf(buffer, "VICTORY!");
        GUI_Text(0, 0, (uint8_t*)buffer, Green, Black);  // Visualizza "PAUSE"
		} else {
        sprintf(buffer, "Time: %d", cnt);
        GUI_Text(0, 0, (uint8_t*)buffer, Red, Black);  // Visualizza il tempo
    }

    if (LPC_TIM0->IR & 1 && game_pause == 1) {  // Verifica se l'interruzione è causata dal Timer 0
        if (cnt == 0 && countPills(screen) > 0) {
							// game over
							game_pause = 2;
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
		sendGameStatus(score, lives, cnt);
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
int game_pause = 0;
int lives = 1;
int cnt_score = 0;
int delay_ghost = 0;
int flag_music = 0;

extern int direction;
volatile int press=0;
volatile int press_1=0;
volatile int press_2=0;
extern int flag_music;

int MAX_DELAY_GHOST = 5;
int cnt_frightened = 80;
int speedup_ghost = 200;

void TIMER1_IRQHandler(void) {
	    static int up = 0, down = 0, sx = 0, dx = 0;
	
		/*************************INT0***************************/
		if(press !=0){
				press++;
				if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
						switch(press){
								case 2:
										 game_pause = (game_pause == 0) ? 1 : 0;  // Se "pause" è 0, lo imposta a 1, altrimenti a 0
											if (game_pause == 0) {
												//disable_timer(1);
											}
											else {
												//enable_timer(1);
											}
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
		
    if (LPC_TIM1->IR & 1) {  // Verifica dell'interrupt
			                     // Controllo per la raccolta delle pills
			if (game_pause == 1) {
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
												frightened_mode = 1;
												cnt_frightened = 90;
        } 
            
        // Aggiorna la posizione di Pac-Man in base alla direzione	
        switch (direction) {
            case UP:
                if (pacman.y > 0 && (screen[pacman.y - 1][pacman.x] !=1)) {//|| screen[pacman.y - 1][pacman.x] == 2 || screen[pacman.y - 1][pacman.x] == 3)) {
										prevPacman = currentPacman;	
										currentPacman = pacMan_up;
                    pacman.y--;
                }
                break;
            case DOWN:
                if (pacman.y < 29 && (screen[pacman.y + 1][pacman.x] != 1 )){//|| screen[pacman.y + 1][pacman.x] == 2 || screen[pacman.y + 1][pacman.x] == 3)) {
                    prevPacman = currentPacman;	
					          currentPacman = pacMan_down;    
								    pacman.y++;
                }
                break;
            case SX:
                if (pacman.x > 0 && (screen[pacman.y][pacman.x - 1] != 1 )){ //|| screen[pacman.y][pacman.x - 1] == 2 || screen[pacman.y][pacman.x - 1] == 3)) {
                    prevPacman = currentPacman;	
					          currentPacman = pacMan_sx;    
								    pacman.x--;
                } else if (pacman.x == 0 && pacman.y == 16) {
                    pacman.x = 23;
                }
                break;
            case DX:
                if (pacman.x < 23 && (screen[pacman.y][pacman.x + 1] != 1 )){// || screen[pacman.y][pacman.x + 1] == 2 || screen[pacman.y][pacman.x + 1] == 3)) {
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
					GUI_Text(0, 300, (uint8_t*)"CCCCCCCCC", Black, Black);  // Visualizza "PAUSE"	
					cnt_score = 0;
				}
			 
				
			if (countPills(screen) == 0){
					game_pause = 3;
				
			}
			

			printLife(lives);
				
			// Controllo collisione tra Pac-Man e fantasma
            if (ghost.x == pacman.x && ghost.y == pacman.y) {
								screen[ghost.x][ghost.y] = 0;
                if (frightened_mode == 1) {  
                    // Pac-Man mangia il fantasma
                    drawIcon(ghost.x * 10, ghost.y * 10, ghost_matrix, Black);
                    cnt_score += 100;
                    ghost.x = 12;  // Torna alla posizione iniziale
                    ghost.y = 16;
                    frightened_mode = 0;  // Esce dalla modalità frightened
                    ghost.previousValue = 0;
										// MAX_DELAY_GHOST = 25;
                } else {
										pacman.x = 12;
										pacman.y = 18;
										direction = NOP;
										delay_ghost = 0;
                    // Fantasma mangia Pac-Man
                    drawIcon(pacman.x * 10, pacman.y * 10, pacMan, Black);
                    lives--;  // Decrementa le vite
                    if (lives == 0) {
                        game_pause = 2;  // Fine del gioco
                    }
                }
            }
						
			if (frightened_mode == 1) {
				cnt_frightened--;
				if (cnt_frightened == 0){
					frightened_mode = 0;
					cnt_frightened = 80;
					MAX_DELAY_GHOST = 5; 
					delay_ghost = MAX_DELAY_GHOST;
				}
			}
			while(delay_ghost == MAX_DELAY_GHOST){
				delay_ghost = 0;
				
				if (frightened_mode == 0) {
					moveGhost(&ghost, &pacman, screen);
				} else {
					MAX_DELAY_GHOST = 10;
					moveGhost_fright(&ghost, &pacman, screen, frightened_mode);
				}
			}

			delay_ghost++;
			
			if (speedup_ghost != 0) {
				speedup_ghost--;
			} else {
				speedup_ghost = 200;
				if (MAX_DELAY_GHOST > 0) MAX_DELAY_GHOST--;
			}
		}
			
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
uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

void TIMER2_IRQHandler(void) {
	 static int sineticks=0;
	/* DAC management */	
	static int currentValue; 
	currentValue = SinTable[sineticks];
	currentValue -= 410;
	currentValue /= 1;
	currentValue += 410;
	LPC_DAC->DACR = currentValue <<6;
	sineticks++;
	if(sineticks==45) sineticks=0;
 
		
	LPC_TIM2->IR = 1;
	return;
	}

void TIMER3_IRQHandler(void) {
	disable_timer(2);
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}
 
/******************************************************************************
**                            End Of File
******************************************************************************/
