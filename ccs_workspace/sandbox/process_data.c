/*
 * process_data.c
 *
 *  Created on: 27.03.2016
 *      Author: mlasch@mailbox.org
 */

#include <csl.h>

#include <sandboxcfg.h>		// includes all the necessary stuff for dsp bios

#include "config_audio_codec.h"
#include "process_data.h"
#include "dsp/dsp_goertzel.h"
#include "dtmf.h"

Int16 rightChannel[108];
Int16 leftChannel[108];

dtmfValueTypeDef dtmfValue;

/*
 * SWI
 */
void processData() {
	Uint32 mailbox = SWI_getmbox();
	Uint32 index = 0;

	/* Handle buffers for ping<->pong operation */
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

	for (index=0;index<108;index++) {
		/* copy right channel */
		rightChannel[index] = gBufferState.cpuRcvBufferPtr[index] & 0xffff;

		/* replay samples on headphone */
		gBufferState.cpuXmtBufferPtr[index] = gBufferState.cpuRcvBufferPtr[index];
	}

	/* generate parameters for dtmf on the right channel */
	dtmfValue.freq_697 = goertzel(rightChannel, 108, 8000, 697);
	dtmfValue.freq_770 = goertzel(rightChannel, 108, 8000, 770);
	dtmfValue.freq_852 = goertzel(rightChannel, 108, 8000, 852);
	dtmfValue.freq_941 = goertzel(rightChannel, 108, 8000, 941);
	dtmfValue.freq_1209 = goertzel(rightChannel, 108, 8000, 1209);
	dtmfValue.freq_1336 = goertzel(rightChannel, 108, 8000, 1336);
	dtmfValue.freq_1477 = goertzel(rightChannel, 108, 8000, 1477);
	dtmfValue.freq_noise = goertzel(rightChannel, 108, 8000, 2000);

	/* send message to dtmf_task */
	SEM_postBinary(&SEM_NewDtmfData);
}
