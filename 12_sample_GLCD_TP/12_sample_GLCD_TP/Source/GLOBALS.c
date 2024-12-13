#include "GLOBALS.h"

// Funzione per disegnare un quadrato pieno 5x5 usando linee orizzontali
void drawSquare(int x, int y, uint16_t color) {
	int i = 0;
  int size = 10; // Larghezza e altezza del quadrato
  for (i = 0; i < size; i++) {
        LCD_DrawLine(x, y + i, x + size - 1, y + i, color); // Disegna 5 linee orizzontali
    }
}

// Funzione per disegnare la matrice sullo schermo LCD
void drawScreenFromMatrix(int screen[32][24]) {
    int pixelSize = 10; // Ogni bit corrisponde a un quadrato 5x5 pixel
		int i = 0;
		int j = 0;
	
    // Ciclo su tutta la matrice
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 24; j++) {
            // Se il bit nella matrice è 1, disegna un quadrato 5x5
            if (screen[i][j] == 1) {
                int x = j * pixelSize;  // Calcola la posizione X (in pixel) per la colonna
                int y = i * pixelSize;  // Calcola la posizione Y (in pixel) per la riga
                drawSquare(x, y, Blue);       // Disegna il quadrato sullo schermo
            }
        }
    }
}
void drawIcon(int x, int y, int icon[10][10], uint16_t color) {
    for (int i = 0; i < 10; i++) {  // Itera su ogni riga della matrice 10x10
        for (int j = 0; j < 10; j++) {
            if (icon[i][j] == 1) {  // Se il pixel è acceso
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
