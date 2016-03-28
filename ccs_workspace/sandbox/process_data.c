/*
 * process_data.c
 *
 *  Created on: 27.03.2016
 *      Author: mlasch@mailbox.org
 */

#include <csl.h>

#include <std.h>
#include <swi.h>

#include "process_data.h"

/*
 * SWI
 */
void processData() {
	Uint32 xxx = SWI_getmbox();
	Uint32 index = 0;

	if (gBufferState.cpuBufferState == StateA) {
		/*
		 * Set cpu buffers to B side
		 */
		gBufferState.cpuRcvBufferPtr = gRcvBufferB;
		gBufferState.cpuXmtBufferPtr = gXmtBufferB;

		gBufferState.cpuBufferState = StateB;
	} else {
		/*
		 * Set cpu buffers to A side
		 */
		gBufferState.cpuRcvBufferPtr = gRcvBufferA;
		gBufferState.cpuXmtBufferPtr = gXmtBufferA;

		gBufferState.cpuBufferState = StateA;
	}

	/* Demo application: swap left-right channel and loop through*/
	for (index=0;index<108;index++) {

		gBufferState.cpuXmtBufferPtr[index] = gBufferState.cpuRcvBufferPtr[index] & 0xffff;
		//gBufferState.cpuXmtBufferPtr[index] = (Uint32)((left<<16)|right);
		//gBufferState.cpuXmtBufferPtr[index] = (Uint32)((right<<16)|left);
	}
}
