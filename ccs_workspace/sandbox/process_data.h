/*
 * process_data.h
 *
 *  Created on: 27.03.2016
 *      Author: marc
 */

#ifndef PROCESS_DATA_H_
#define PROCESS_DATA_H_

extern Uint32 gRcvBufferA[108];
extern Uint32 gRcvBufferB[108];
extern Uint32 *gCurrentBuffer;

void processData(void);

#endif /* PROCESS_DATA_H_ */
