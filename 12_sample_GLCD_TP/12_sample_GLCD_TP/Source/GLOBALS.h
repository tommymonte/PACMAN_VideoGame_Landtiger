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

extern int row;
extern int col;

// Definizione delle direzioni possibili
#define UP 1
#define DOWN 2
#define SX 3
#define DX 4
#define NOP 0

extern void drawSquare(int x, int y, uint16_t color);
extern void drawScreenFromMatrix(int screen[32][24]);
extern void drawIcon(int x, int y, int pacMan[10][10], uint16_t color);
extern void drawPacmanAt(int screenX, int screenY, int pacMan[10][10], uint16_t color);
extern void drawPacman(void);
extern void drawPill(void);
extern void dristibutrPills(void);

// Definizione della struttura per Pac-Man
typedef struct {
    int x;  // Posizione X nella matrice screen
    int y;  // Posizione Y nella matrice screen
} PacMan;

// Dichiarazione della variabile globale Pac-Man
extern PacMan pacman;

#endif
