#include "GLOBALS.h"
#include <stdio.h>
// Funzione per disegnare un quadrato pieno 5x5 usando linee orizzontali
void drawSquare(int x, int y, uint16_t color) {
	int i = 0;
  int size = 10; // Larghezza e altezza del quadrato
  for (i = 0; i < size; i++) {
        LCD_DrawLine(x, y + i, x + size - 1, y + i, color); // Disegna 5 linee orizzontali
    }
}

// Funzione per disegnare la matrice sullo schermo LCD
// Funzione per disegnare la matrice sullo schermo LCD
void drawScreenFromMatrix(int screen[32][24], int pill[10][10], int powerPill[10][10]) {
    int pixelSize = 10; // Ogni cella corrisponde a un quadrato di 10x10 pixel

    // Ciclo su tutta la matrice
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 24; j++) {
            int x = j * pixelSize;  // Calcola la posizione X (in pixel) per la colonna
            int y = i * pixelSize;  // Calcola la posizione Y (in pixel) per la riga

            // Disegna in base al valore della cella
            switch (screen[i][j]) {
                case 1:
                    drawSquare(x, y, Blue);           // Disegna muro
                    break;
                case 2:
										drawIcon(x, y, pill, Magenta);    
                    break;
                case 3:
                    drawIcon(x, y, powerPill, Magenta);
                    break;
                default:
                    break;  // Celle vuote (valore 0), non disegnare nulla
            }
        }
    }
}

void drawIcon(int x, int y, int icon[10][10], uint16_t color) {
    for (int i = 0; i < 10; i++) {  // Itera su ogni riga della matrice 10x10
        for (int j = 0; j < 10; j++) {
            if (icon[i][j] == 1) {  // Se il pixel � acceso
                LCD_DrawLine(x + j, y + i, x + j, y + i, color);  // Disegna un punto
            }
        }
    }
}

void drawPacmanAt(int screenX, int screenY, int pacMan[10][10], uint16_t color) {
    int displayX = screenX * 10;  // Converti la cella logica in coordinate fisiche
    int displayY = screenY * 10;
    drawIcon(displayX, displayY, pacMan, color);
}

// Funzione per disegnare Pac-Man
void drawPacman(void) {
    static int last_x = 0, last_y = 0;  // Posizione pre edente di Pac-Man

    // Cancella la posizione precedente di Pac-Man
			drawPacmanAt(last_x, last_y, prevPacman, Black);
    // Disegna Pac-Man nella nuova posizione
			drawPacmanAt(pacman.x, pacman.y, currentPacman, Yellow);
		//drawIcon(pacman.x * 10, pacman.y * 10, currentPacman, Black);

    // Aggiorna la posizione precedente
    last_x = pacman.x;
    last_y = pacman.y;
}

void distributePills(uint8_t *lfsr_register, int screen[32][24]) {
    int random_row, random_col;
		int flag = 0;
	
		while (flag == 0) {
			// Genera una posizione casuale usando LFSR
			random_row = (lfsr(*lfsr_register) % 32);  // Riga tra 0 e 31
			*lfsr_register = lfsr(*lfsr_register);     // Aggiorna l'LFSR
			random_col = (lfsr(*lfsr_register) % 24);  // Colonna tra 0 e 23
			*lfsr_register = lfsr(*lfsr_register);     // Aggiorna l'LFSR

			// Se il valore in quella posizione è 2, posiziona la pillola
			if (screen[random_row][random_col] == 2) {
					screen[random_row][random_col] = 3;  // Imposta la pillola
					drawIcon(random_col * 10, random_row * 10, powerPill, Magenta);  // Disegna la pillola
					flag = 1;
			}
		}
	}

// Funzione LFSR per la generazione pseudo-casuale
uint8_t lfsr(uint8_t lfsr_register) {
    uint8_t feedback = (lfsr_register & 0x01) ^ ((lfsr_register >> 3) & 0x01);  // Polinomio x^4 + x + 1
    lfsr_register >>= 1;
    if (feedback) {
        lfsr_register |= 0x80;  // Aggiungi feedback al bit più significativo
    }
    return lfsr_register;
}


// Funzione per contare il numero di 2 nella matrice
int countPills(int screen[32][24]) {
    int count = 0;  // Variabile per contare i 2
    
    // Scorri tutta la matrice
    for (int row = 0; row < 32; row++) {
        for (int col = 0; col < 24; col++) {
            if (screen[row][col] == 2 || screen[row][col] == 3) {
                count++;  // Incrementa il contatore se trovi un 2
            }
        }
    }

    return count;  // Restituisce il numero di pillole (2)
}

// Funzione per contare il numero di 2 nella matrice
int countPwPills(int screen[32][24]) {
    int count = 0;  // Variabile per contare i 2
    
    // Scorri tutta la matrice
    for (int row = 0; row < 32; row++) {
        for (int col = 0; col < 24; col++) {
            if (screen[row][col] == 2 || screen[row][col] == 3 ) {
                count++;  // Incrementa il contatore se trovi un 2
            }
        }
    }

    return count;  // Restituisce il numero di pillole (2)
}

void printScore(countPills){
    char buffer[30];                   // Buffer per il testo da stampare
		// Aggiorna il display con il valore del contatore
    sprintf(buffer, "SCORE: %d", countPills);
    GUI_Text(120, 0, (uint8_t*)buffer, White, Black);
		
}

void initGame(void) {

}
