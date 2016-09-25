/*
 * globals.c
 *
 *  Created on: 23.09.2016
 *      Author: marc
 */

#include <csl.h>

#include "globals.h"


/*
 * DTMF string
 */
unsigned char gDigitList[DIGIT_MAX];

/*
 * Buffers
 */
Uint32 gRcvBufferA[108];
Uint32 gRcvBufferB[108];
Uint32 gXmtBufferA[108];
Uint32 gXmtBufferB[108];

BufferStateTypeDef gBufferState;

Uint8 gTccRcvChan;
Uint8 gTccXmtChan;

Uint16 gRcvFlag = 0, gXmtFlag = 0;

dtmfValueTypeDef dftResult;

unsigned char gNewDigit;

void globals_init() {
	gDigitList[0] = '\0';
}
