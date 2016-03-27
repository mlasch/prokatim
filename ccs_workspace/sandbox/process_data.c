/*
 * process_data.c
 *
 *  Created on: 27.03.2016
 *      Author: marc
 */

#include <csl.h>

#include <std.h>
#include <swi.h>

#include "config_audio_codec.h"
#include "process_data.h"

/*
 * SWI
 */
void processData() {
	Uint32 mailbox = SWI_getmbox();
	Uint32 sum;
	Uint32 i;

	if (gCurrentBuffer == gRcvBufferA) {
		gCurrentBuffer = gRcvBufferB;
	} else {
		gCurrentBuffer = gRcvBufferA;
	}

	for (i=0;i<108;i++) {
		sum += gCurrentBuffer[i];
	}

}
