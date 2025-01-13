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
#include "../GLOBALS.h"
extern uint8_t icr ; 										//icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;
extern CAN_msg       CAN_TxMsg;    /* CAN message for sending */
extern CAN_msg       CAN_RxMsg;    /* CAN message for receiving */                                



/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler(void) {
		icr = 0;
    uint8_t icr;

    // Check CAN2 interrupt
    icr = (LPC_CAN2->ICR | icr) & 0xFF;
    if (icr & (1 << 0)) {  // Message received on CAN2
        CAN_msg CAN_RxMsg;
        CAN_rdMsg(2, &CAN_RxMsg);
        LPC_CAN2->CMR = (1 << 2);  // Release receive buffer

        // Decode the received message
        uint16_t score = (CAN_RxMsg.data[0] << 8) | CAN_RxMsg.data[1];
        uint8_t lives = CAN_RxMsg.data[2];
        uint8_t countdown = CAN_RxMsg.data[3];
				
				printScore(score);
				printLife(lives);

    }
}

