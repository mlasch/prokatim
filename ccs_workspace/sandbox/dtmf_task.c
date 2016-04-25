/*
 * dtmf_task.c
 *
 *  Created on: 08.04.2016
 *      Author: marc
 */

#include <sandboxcfg.h>		// includes all the necessary stuff for dsp bios

#include "dtmf.h"

char dtmfChar[4][3] = {{'1', '2', '3'},
					{'4', '5', '6'},
					{'7', '8', '9'},
					{'*', '0', '#'}};

Uint8 gCurrentChar;

/* DTMF table
 *       1209  1336  1477
 * 697	   1     2	   3
 * 770     4     5     6
 * 852     7     8     9
 * 941     *     0     #
 */

void dtmfTask() {
	/* Handle DTMF state machine */

	while(1) {
		int i;
		float *ptr = (float*) &dtmfValue;
		Uint16 row, col;
		float row_f, col_f;
		float temp = 0.0, new_value = 0.0;

		/* wait for new calculations */
		SEM_pendBinary(&SEM_NewDtmfData, SYS_FOREVER);

		// select row
		for (i=0;i<4;i++) {
			new_value = *(ptr+i);
			if (new_value > temp) {
				row = i;
				row_f = new_value;
			}
			temp = *(ptr+i);
		}

		// select column
		for (i=0;i<3;i++) {
			new_value = *(ptr+i+4);
			if (new_value > temp) {
				col = i;
				col_f = new_value;
			}
			temp = *(ptr+i+4);
		}

		// verify if row and col are over noise the floor
		if (row_f - dtmfValue.freq_noise > 6.8 && col_f - dtmfValue.freq_noise > 6.8) {
			gCurrentChar = dtmfChar[row][col];
			SEM_postBinary(&SEM_NewChar);
		}
	}
}
