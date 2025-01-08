/*----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC17xx.h>                  /* LPC17xx definitions */
#include "CAN.h"                      /* LPC17xx CAN adaption layer */
#include "../GLCD/GLCD.h"

extern uint8_t icr ; 										//icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;
extern CAN_msg       CAN_TxMsg;    /* CAN message for sending */
extern CAN_msg       CAN_RxMsg;    /* CAN message for receiving */                                

static int puntiRicevuti1 = 0;
static int puntiInviati1 = 0;

static int puntiRicevuti2 = 0;
static int puntiInviati2 = 0;

uint16_t val_RxCoordX = 0;            /* Locals used for display */
uint16_t val_RxCoordY = 0;

/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler (void)  {
    uint16_t score;      // Variabile per il punteggio
    uint8_t lives;        // Variabile per le vite

    /* check CAN controller 1 */
    icr = 0;
    icr = (LPC_CAN1->ICR | icr) & 0xFF;               /* clear interrupts */

    // Ricezione CAN Controller #1
    if (icr & (1 << 0)) {                            /* Messaggio ricevuto */
        CAN_rdMsg(1, &CAN_RxMsg);                    /* Leggi il messaggio */
        LPC_CAN1->CMR = (1 << 2);                    /* Rilascia il buffer di ricezione */

        // Estrai punteggio (16 bit) e vite (8 bit)
        score = (CAN_RxMsg.data[0] << 8) | CAN_RxMsg.data[1];  // Byte 0 e 1
        lives = CAN_RxMsg.data[2];                              // Byte 2
    }

    // Trasmissione CAN Controller #1
    if (icr & (1 << 1)) {                          /* Messaggio trasmesso */
        // Esempio: trasmissione di punteggio e vite
        CAN_TxMsg.id = 0x123;                      // Imposta un ID del messaggio
        CAN_TxMsg.len = 3;                         // Lunghezza dei dati (punti 2 byte, vite 1 byte)
        
        // Inserisci punteggio e vite nel messaggio
        CAN_TxMsg.data[0] = (score >> 8) & 0xFF;   // Byte alto del punteggio
        CAN_TxMsg.data[1] = lives & 0xFF;          // Byte basso del punteggio
        CAN_TxMsg.data[2] = lives;                  // Byte delle vite

        CAN_wrMsg(1, &CAN_TxMsg);                  // Scrivi il messaggio sul bus CAN

        puntiInviati1++;
    }

    /* check CAN controller 2 */
    icr = 0;
    icr = (LPC_CAN2->ICR | icr) & 0xFF;             /* clear interrupts */

    // Ricezione CAN Controller #2
    if (icr & (1 << 0)) {                           /* Messaggio ricevuto */
        CAN_rdMsg(2, &CAN_RxMsg);                   /* Leggi il messaggio */
        LPC_CAN2->CMR = (1 << 2);                   /* Rilascia il buffer di ricezione */

        // Estrai punteggio (16 bit) e vite (8 bit)
        score = (CAN_RxMsg.data[0] << 8) | CAN_RxMsg.data[1];  // Byte 0 e 1
        lives = CAN_RxMsg.data[2];                              // Byte 2
    }

    // Trasmissione CAN Controller #2
    if (icr & (1 << 1)) {                          /* Messaggio trasmesso */
        // Esempio: trasmissione di punteggio e vite
        CAN_TxMsg.id = 0x123;                      // Imposta un ID del messaggio
        CAN_TxMsg.len = 3;                         // Lunghezza dei dati (punti 2 byte, vite 1 byte)
        
        // Inserisci punteggio e vite nel messaggio
        CAN_TxMsg.data[0] = (score >> 8) & 0xFF;   // Byte alto del punteggio
        CAN_TxMsg.data[1] = score & 0xFF;          // Byte basso del punteggio
        CAN_TxMsg.data[2] = lives;                  // Byte delle vite

        CAN_wrMsg(2, &CAN_TxMsg);                  // Scrivi il messaggio sul bus CAN

        puntiInviati2++;
    }
}

