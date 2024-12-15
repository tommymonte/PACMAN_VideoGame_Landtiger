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
            int x = j * pixelSize;  // Calcola la posizione X (in pixel) per la colonna
            int y = i * pixelSize;  // Calcola la posizione Y (in pixel) per la riga
            if (screen[i][j] == 1) {
                drawSquare(x, y, Blue);       // Disegna il quadrato sullo schermo
            } /*else if (screen[i][j] == 2) {
								drawPillAt(x, y, pill, Magenta);
						} else if (screen[i][j] == 3) {
								drawPowerPill(x, y, powerPill, Green); 
								} */
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
    drawPacmanAt(last_x, last_y, pacMan, Black);

    // Disegna Pac-Man nella nuova posizione
    drawPacmanAt(pacman.x, pacman.y, pacMan, Yellow);

    // Aggiorna la posizione precedente
    last_x = pacman.x;
    last_y = pacman.y;
}

void drawPillAt(int screenX, int screenY, int pill[10][10], uint16_t color) {
    int displayX = screenX * 10;  // Converti la cella logica in coordinate fisiche
    int displayY = screenY * 10;
    drawIcon(displayX, displayY, pill, color);
}

void drawPowerPillAt(int screenX, int screenY, int powerPill[10][10], uint16_t color) {
    int displayX = screenX * 10;  // Converti la cella logica in coordinate fisiche
    int displayY = screenY * 10;
    drawIcon(displayX, displayY, powerPill, color);
}

void distributePills(void) {
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 24; j++) {
            if (screen[i][j] == 2) {
                drawPillAt(i, j, pill, Magenta);  // Disegna una pillola normale
            } else if (screen[i][j] == 3) {
                drawPowerPillAt(i, j, powerPill, Green);  // Disegna una pillola speciale (vita)
            }
        }
    }
}

void initGame(void) {
    distributePills();     // Disegna tutte le pills sulla mappa
}
