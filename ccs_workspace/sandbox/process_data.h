/*
 * process_data.h
 *
 *  Created on: 27.03.2016
 *      Author: mlasch@mailbox.org
 */

#ifndef PROCESS_DATA_H_
#define PROCESS_DATA_H_

#include <string.h>

#define DTMF_LEN 7

enum {
	StateA, StateB
};

typedef struct {
	Uint32 *cpuRcvBufferPtr;
	Uint32 *cpuXmtBufferPtr;
	Uint32 cpuBufferState;
} BufferStateTypeDef;

typedef struct {
	float freq_697;
	float freq_770;
	float freq_852;
	float freq_941;
	float freq_1209;
	float freq_1336;
	float freq_1477;
	float freq_noise;
} dtmfValueTypeDef;

extern unsigned char dtmfDigits[4][3];

void processData(void);
void sort_freq(float*, float*, size_t length);
#endif /* PROCESS_DATA_H_ */
