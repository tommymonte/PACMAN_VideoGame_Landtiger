/****************************************Copyright (c)****************************************************
**--------------------------------------------------------------------------------------------------------
** Modified by:             Tommaso Montedoro
** Version:                 v1.0
** Descriptions:            first Assignment
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "GameFeatures/matrixGame.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

// Funzione per disegnare un quadrato 5x5 usando linee
void drawSquare(int x, int y) {
    // Disegna le 4 linee del quadrato
    LCD_DrawLine(x, y, x + 4, y, Blue2);       // Linea superiore (orizzontale)
    LCD_DrawLine(x, y, x, y + 4, Blue2);       // Linea sinistra (verticale)
    LCD_DrawLine(x, y + 4, x + 4, y + 4, Blue2); // Linea inferiore (orizzontale)
    LCD_DrawLine(x + 4, y, x + 4, y + 4, Blue2); // Linea destra (verticale)
}

// Funzione per disegnare la matrice sullo schermo LCD
void drawScreenFromMatrix(int screen[64][48]) {
    int pixelSize = 5; // Ogni bit corrisponde a un quadrato 5x5 pixel
		int i = 0;
		int j = 0;
	
    // Ciclo su tutta la matrice
    for (i = 0; i < 64; i++) {
        for (j = 0; j < 48; j++) {
            // Se il bit nella matrice è 1, disegna un quadrato 5x5
            if (screen[i][j] == 1) {
                int x = j * pixelSize;  // Calcola la posizione X (in pixel) per la colonna
                int y = i * pixelSize;  // Calcola la posizione Y (in pixel) per la riga
                drawSquare(x, y);       // Disegna il quadrato sullo schermo
            }
        }
    }
}

int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();
	LCD_Clear(Black);
	drawScreenFromMatrix(screen);
	//init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	//init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	//init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	init_timer(0, 0xC8 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */
	
	enable_timer(0);
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
