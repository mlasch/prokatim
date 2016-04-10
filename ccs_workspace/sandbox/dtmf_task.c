/*
 * dtmf_task.c
 *
 *  Created on: 08.04.2016
 *      Author: marc
 */

#include <sandboxcfg.h>		// includes all the necessary stuff for dsp bios

#include "dtmf.h"

void dtmfTask() {
	/* Handle DTMF state machine */
	while(1) {
		/* wait for new calculations */
		SEM_pendBinary(&SEM_NewDtmfData, SYS_FOREVER);
		asm(" nop");

	}
}
