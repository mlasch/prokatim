/*
 * process_data.h
 *
 *  Created on: 27.03.2016
 *      Author: mlasch@mailbox.org
 */

#ifndef PROCESS_DATA_H_
#define PROCESS_DATA_H_

#include "config_audio_codec.h"

extern Uint32 gRcvBufferA[];
extern Uint32 gRcvBufferB[];
extern Uint32 gXmtBufferA[];
extern Uint32 gXmtBufferB[];

extern BufferStateTypeDef gBufferState;

void processData(void);

#endif /* PROCESS_DATA_H_ */
