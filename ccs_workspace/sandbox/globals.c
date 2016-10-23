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
int gDigitIndex;

/*
 * Buffers
 */
Uint32 gRcvBufferA[108];
Uint32 gRcvBufferB[108];
Uint32 gXmtBufferA[108];
Uint32 gXmtBufferB[108];

Int16 rightChannel[108];
//Int16 leftChannel[108];

BufferStateTypeDef gBufferState;

Uint8 gTccRcvChan;
Uint8 gTccXmtChan;

Uint16 gRcvFlag = 0, gXmtFlag = 0;

dtmfValueTypeDef dftResult;

unsigned char gNewDigit;

int gNewDigitCounter = 0;

void globals_init() {
	gDigitList[0] = '\0';
	gDigitIndex = 0;
}
