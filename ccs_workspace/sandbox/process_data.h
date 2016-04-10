/*
 * process_data.h
 *
 *  Created on: 27.03.2016
 *      Author: mlasch@mailbox.org
 */

#ifndef PROCESS_DATA_H_
#define PROCESS_DATA_H_

enum {
	StateA, StateB
};

typedef struct {
	Uint32 *cpuRcvBufferPtr;
	Uint32 *cpuXmtBufferPtr;
	Uint32 cpuBufferState;
} BufferStateTypeDef;

extern BufferStateTypeDef gBufferState;

extern Uint32 gRcvBufferA[];
extern Uint32 gRcvBufferB[];
extern Uint32 gXmtBufferA[];
extern Uint32 gXmtBufferB[];

extern Int16 rightChannel[];
extern Int16 leftChannel[];

void processData(void);

#endif /* PROCESS_DATA_H_ */
