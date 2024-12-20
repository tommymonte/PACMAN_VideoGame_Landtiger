#ifndef GLOBALS_H
#define GLOBALS_H

#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "GameFeatures/matrixGame.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"

// Dichiarazione della variabile globale direction
extern int direction;
extern int chase_mode;
extern int frightened_mode;
extern int row;
extern int col;

// Definizione delle direzioni possibili
#define UP 1
#define DOWN 2
#define SX 3
#define DX 4
#define NOP 0

#define N_PILLS 240
#define N_PwPILLS 6

extern int lives;
extern int score;
extern int totalPillsPlaced;
extern int pause;

/*funzioni di gioco per PACMAN*/
extern void drawSquare(int x, int y, uint16_t color);
extern void drawScreenFromMatrix(int screen[32][24], int pill[10][10], int powerPill[10][10]);
extern void drawIcon(int x, int y, int pacMan[10][10], uint16_t color);
extern void drawPacmanAt(int screenX, int screenY, int pacMan[10][10], uint16_t color);
extern void drawPacman(void);
extern void drawPill(void);
extern void dristibutePills(uint8_t lfsr_register, int screen[32][24]);
extern void drawPillAt(int screenX, int screenY, int pill[10][10], uint16_t color);
extern void drawPowerPillAt(int screenX, int screenY, int pill[10][10], uint16_t color);
uint8_t lfsr(uint8_t lfsr_register);
extern int countPills(screen[32][24]);
extern int countPwPills(screen[32][24]);
extern void printScore(int countPills);
extern void printLife(int lives);
extern void life(int lives);

// Definizione della struttura per Pac-Man
typedef struct {
    int x;  // Posizione X nella matrice screen
    int y;  // Posizione Y nella matrice screen
} PacMan;

extern int (*currentPacman)[10];
extern int (*prevPacman)[10];

// Dichiarazione della variabile globale Pac-Man
extern PacMan pacman;

/****************GHOST FUNCTION******************/
// Definizione della struttura per Pac-Man
typedef struct {
    int x;  // Posizione X nella matrice screen
    int y;  // Posizione Y nella matrice screen
	  int previousValue; // Valore originale della cella (es. 0, 2, 3)
} Ghost;

extern Ghost ghost;



#endif
