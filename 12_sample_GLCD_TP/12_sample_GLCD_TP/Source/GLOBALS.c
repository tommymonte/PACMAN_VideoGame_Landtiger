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
            } else if (icon[i][j] == 2) {
							LCD_DrawLine(x + j, y + i, x + j, y + i, White);  // Disegna un punto
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

void distributePills(int score, int screen[32][24]) {
    int random_row, random_col;
		int flag = 0;
		int new_score = score;
	
		while (flag == 0) {
			// Genera una posizione casuale usando LFSR
			random_row = ( new_score % 32 );  // Riga tra 0 e 31
			//*lfsr_register = lfsr(*lfsr_register);     // Aggiorna l'LFSR
			random_col = ( new_score % 24 );  // Colonna tra 0 e 23
			//*lfsr_register = lfsr(*lfsr_register);     // Aggiorna l'LFSR
			new_score++;
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

void printScore(int countPills){
    char buffer[30];                   // Buffer per il testo da stampare
		// Aggiorna il display con il valore del contatore
    sprintf(buffer, "SCORE: %d", countPills);
    GUI_Text(120, 0, (uint8_t*)buffer, White, Black);
}

void life( int lives) {
		char buffer[10];                   // Buffer per il testo da stampare
		// Aggiorna il display con il valore del contatore
    sprintf(buffer, "vite: %d", lives);
    GUI_Text(0, 310, (uint8_t*)buffer, White, Black);
}

void printLife( int lives){
	int x_life = 0;
	for ( int j = 0; j<5; j++ ){
					drawIcon(x_life*10, 300, heart, Black); 
					x_life++;
				}
	x_life = 0;
	for ( int i = 0; i<lives; i++ ){
					drawIcon(x_life*10, 300, heart, Red); 
					x_life++;
				}
	x_life = 0;
	}

void initGame(void) {

}


void moveGhost(Ghost *ghost, PacMan *pacman, int screen[32][24]) {
    int current_x = ghost->x;
    int current_y = ghost->y;
    int target_x = pacman->x;
    int target_y = pacman->y;

    // Calcola la distanza Manhattan nelle 4 direzioni
    int dx[] = {0, 0, -1, 1};  // Su, Giù, Sinistra, Destra
    int dy[] = {-1, 1, 0, 0};

    int min_distance = 1000;  // Un valore grande per inizializzare
    int next_x = current_x;
    int next_y = current_y;

    // Controlla tutte le 4 direzioni possibili
    for (int i = 0; i < 4; i++) {
        int new_x = current_x + dx[i];
        int new_y = current_y + dy[i];

        // Verifica che la nuova posizione sia valida (non oltre i limiti e non un muro)
        if (new_x >= 0 && new_x < 32 && new_y >= 0 && new_y < 24 && screen[new_y][new_x] != 1) {
            int distance = abs(target_x - new_x) + abs(target_y - new_y);  // Distanza Manhattan

            if (distance < min_distance) {
                min_distance = distance;
                next_x = new_x;
                next_y = new_y;
            }
        }
    }

    // Ripristina il valore della cella precedente
    screen[current_y][current_x] = ghost->previousValue;
    if (ghost->previousValue == 2) {
				drawIcon(current_x * 10, current_y * 10, ghost_matrix, Black); // Ripristina una cella vuota
        drawIcon(current_x * 10, current_y * 10, pill, Magenta); // Ripristina la pillola
    } else if (ghost->previousValue == 0 || ghost->previousValue == 4) {
        drawIcon(current_x * 10, current_y * 10, ghost_matrix, Black); // Ripristina una cella vuota
    } else if (ghost->previousValue == 3) {
        drawIcon(current_x * 10, current_y * 10, ghost_matrix, Black); // Ripristina una cella vuota
        drawIcon(current_x * 10, current_y * 10, powerPill, Magenta); // Ripristina la pillola
    }

    // Salva il valore della nuova cella
    ghost->previousValue = screen[next_y][next_x];

    // Aggiorna la posizione del fantasmino sulla griglia
    ghost->x = next_x;
    ghost->y = next_y;
    screen[next_y][next_x] = 4;  // Assegna un valore specifico per il fantasmino

    // Disegna il fantasmino nella nuova posizione
    drawIcon(next_x * 10, next_y * 10, ghost_matrix, Red);
}

void moveGhost_fright(Ghost *ghost, PacMan *pacman, int screen[32][24], int frightened_mode) {
    int current_x = ghost->x;
    int current_y = ghost->y;
    int target_x = pacman->x;
    int target_y = pacman->y;

    // Direzioni possibili: Su, Giù, Sinistra, Destra
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    int best_distance = frightened_mode ? -1 : 1000; // In modalità frightened cerchiamo la distanza massima
    int next_x = current_x;
    int next_y = current_y;

    // Controlla tutte le 4 direzioni possibili
    for (int i = 0; i < 4; i++) {
        int new_x = current_x + dx[i];
        int new_y = current_y + dy[i];

        // Verifica che la nuova posizione sia valida (non oltre i limiti e non un muro)
        if (new_x >= 0 && new_x < 32 && new_y >= 0 && new_y < 24 && screen[new_y][new_x] != 1) {
            int distance = abs(target_x - new_x) + abs(target_y - new_y); // Distanza Manhattan

            if (frightened_mode == 1) {
                // Se il fantasmino è in modalità frightened, cerca di massimizzare la distanza
                if (distance > best_distance) {
                    best_distance = distance;
                    next_x = new_x;
                    next_y = new_y;
                }
            } else {
                // Se il fantasmino insegue Pac-Man, cerca di minimizzare la distanza
                if (distance < best_distance) {
                    best_distance = distance;
                    next_x = new_x;
                    next_y = new_y;
                }
            }
        }
    }

    // Ripristina il valore della cella precedente
    screen[current_y][current_x] = ghost->previousValue ;
    if (ghost->previousValue == 2) {
				drawIcon(current_x * 10, current_y * 10, ghost_matrix, Black); // Ripristina una cella vuota
        drawIcon(current_x * 10, current_y * 10, pill, Magenta); // Ripristina la pillola
    } else if (ghost->previousValue == 0 || ghost->previousValue == 4) {
        //drawIcon(current_x * 10, current_y * 10, pill, Black); // Ripristina una cella vuota
				drawIcon(current_x * 10, current_y * 10, ghost_matrix, Black); // Ripristina una cella vuota
    } else if (ghost->previousValue == 3) {
        drawIcon(current_x * 10, current_y * 10, ghost_matrix, Black); // Ripristina una cella vuota
        drawIcon(current_x * 10, current_y * 10, powerPill, Magenta); // Ripristina la pillola
    }

    // Salva il valore della nuova cella
    ghost->previousValue = screen[next_y][next_x];

    // Aggiorna la posizione del fantasmino sulla griglia
    ghost->x = next_x;
    ghost->y = next_y;
    screen[next_y][next_x] = 4; // Assegna un valore specifico per il fantasmino
		if (frightened_mode == 0) {
    // Disegna il fantasmino nella nuova posizione
    drawIcon(next_x * 10, next_y * 10, ghost_matrix ,Red);
		} else {
			drawIcon(next_x * 10, next_y * 10, ghost_matrix, Blue2);
		}
}

void sendGameStatus(uint16_t score, uint8_t lives, uint8_t countdown) {
    CAN_msg CAN_TxMsg;

    // Encode the message
    CAN_TxMsg.id = 0x123;                         // Message ID
    CAN_TxMsg.len = 4;                            // Data length (5 bytes)
    CAN_TxMsg.data[0] = score & 0xFF;        // High byte of Score
    CAN_TxMsg.data[1] = (score >> 8) & 0xFF;            // Low byte of Score
    CAN_TxMsg.data[2] = lives;                    // Remaining Lives
    CAN_TxMsg.data[3] = countdown ;
		CAN_TxMsg.format=STANDARD_FORMAT;
		CAN_TxMsg.type=DATA_FRAME;
    // Transmit the message on CAN1
    CAN_wrMsg(1, &CAN_TxMsg);
}


