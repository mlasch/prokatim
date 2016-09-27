/*
 * globals.h
 *
 *  Created on: 23.09.2016
 *      Author: marc
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "process_data.h"

#define DIGIT_MAX 20

extern unsigned char gDigitList[DIGIT_MAX];
extern int gDigitIndex;

extern BufferStateTypeDef gBufferState;

extern Uint32 gRcvBufferA[];
extern Uint32 gRcvBufferB[];
extern Uint32 gXmtBufferA[];
extern Uint32 gXmtBufferB[];

extern Int16 rightChannel[];
extern Int16 leftChannel[];

extern Uint8 gTccRcvChan;
extern Uint8 gTccXmtChan;

extern Uint16 gRcvFlag, gXmtFlag;

extern dtmfValueTypeDef dftResult;

extern unsigned char gNewDigit;
extern int gNewDigitCounter;

void globals_init();

#endif /* GLOBALS_H_ */
