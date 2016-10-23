/*
 * process_data.c
 *
 *  Created on: 27.03.2016
 *      Author: mlasch@mailbox.org
 */

#include <csl.h>
#include <csl_edma.h>

#include <sandboxcfg.h>		// includes all the necessary stuff for dsp bios

#include "config_audio_codec.h"
#include "process_data.h"
#include "dsp/dsp_goertzel.h"

#include "globals.h"

unsigned char dtmfDigits[4][3] = {
		{'1', '2', '3'},
		{'4', '5', '6'},
		{'7', '8', '9'},
		{'*', '0', '#'}};

/*
 * HWI interrupt routing to process buffer
 */
void EDMA_service_routine() {
	Uint32 index = 0;

	if (EDMA_intTest(gTccRcvChan)) {
		EDMA_intClear(gTccRcvChan);
		/* start software interrupt to process buffer */
		gRcvFlag = 1;
	}
	if (EDMA_intTest(gTccXmtChan)) {
		EDMA_intClear(gTccXmtChan);
		/* start software interrupt to process buffer */
		gXmtFlag = 1;
	}

	if (gXmtFlag && gRcvFlag) {
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
		dftResult.freq_noise = goertzel(rightChannel, 108, 8000, 1600);

		dftResult.freq_697 = goertzel(rightChannel, 108, 8000, 697);
		dftResult.freq_770 = goertzel(rightChannel, 108, 8000, 770);
		dftResult.freq_852 = goertzel(rightChannel, 108, 8000, 852);
		dftResult.freq_941 = goertzel(rightChannel, 108, 8000, 941);
		dftResult.freq_1209 = goertzel(rightChannel, 108, 8000, 1209);
		dftResult.freq_1336 = goertzel(rightChannel, 108, 8000, 1336);
		dftResult.freq_1477 = goertzel(rightChannel, 108, 8000, 1477);

		/* calculate ref level */

		dftResult.freq_697 = dftResult.freq_697 / dftResult.freq_noise;
		dftResult.freq_770 = dftResult.freq_770 / dftResult.freq_noise;
		dftResult.freq_852 = dftResult.freq_852 / dftResult.freq_noise;
		dftResult.freq_941 = dftResult.freq_941 / dftResult.freq_noise;
		dftResult.freq_1209 = dftResult.freq_1209 / dftResult.freq_noise;
		dftResult.freq_1336 = dftResult.freq_1336 / dftResult.freq_noise;
		dftResult.freq_1477 = dftResult.freq_1477 / dftResult.freq_noise;

		SWI_or(&SWI_process_data, 0x00);

		gRcvFlag = 0;
		gXmtFlag = 0;
	}
}

/*
 * SWI for DTMF detection
 */
void processData() {
	Uint32 mailbox = SWI_getmbox();
	float snr_limit;
	float temp;
	float sortResult[DTMF_LEN];
	int match_freq[DTMF_LEN];
	static int freq1, freq2;
	int match_num = 0;
	int i, dtmf_row, dtmf_col;
	static int state = 0;

	sort_freq((float*) &dftResult, sortResult, 7);

	snr_limit = (
			sortResult[0]/5 +
			sortResult[1]/5 +
			sortResult[2]/5 +
			sortResult[3]/5 +
			sortResult[4]/5
			) * 20;

	for (i=0;i<DTMF_LEN;i++) {
		temp = *(((float *)&dftResult)+i);
		if ( temp > snr_limit) {
			// detected a frequency over threshold
			match_freq[match_num] = i;
			match_num++;
		}
	}

	switch (state) {
	case 0:
		if (match_num == 2) {
			freq1 = match_freq[0];
			freq2 = match_freq[1];
			state = 1;
		} else
			state = 0;

		break;
	case 1:
		if (match_num == 2 &&
				freq1 == match_freq[0] &&
				freq2 == match_freq[1])
			state = 2;
		else
			state = 0;

		break;
	case 2:
		if (match_num == 2 &&
				freq1 == match_freq[0] &&
				freq2 == match_freq[1])
			state = 3;
		else
			state = 0;

		break;
	case 3:
		if (match_num < 2)
			state = 4;
		else
			state = 3;

		break;
	case 4:
		if (match_num < 2)
			state = 5;
		else
			state = 0;

		break;
	case 5:
		/*
		 * Find frequencies
		 * check weather there is a row and a column, else restart recognation
		 *
		 */

		state = 0;

		if (freq1 <= 3) {
			// freq0 -> row
			if (freq2 > 3) {
				// freq1 -> col
				dtmf_row = freq1;
				dtmf_col = freq2 - 4;
			} else {
				break;
			}
		} else {
			// freq0 -> col
			if (match_freq[1] <= 3) {
				// freq1 -> row
				dtmf_row = freq2;
				dtmf_col = freq1 - 4;
			} else {
				break;
			}
		}

		/* recognized new digit, add digit to string */
		gDigitList[gDigitIndex] = dtmfDigits[dtmf_row][dtmf_col];
		gDigitList[gDigitIndex + 1] = '\0';
		gDigitIndex++;

		gNewDigitCounter = 0;

		break;
	}
}

void sort_freq(float* dft, float* sortResult, size_t length) {
	float temp;
	int i,j;

	memcpy(sortResult, dft, sizeof(float)*length);

	for (i = 0; i<length; i++) {
		for (j = i+1; j<length;j++) {
			if (*(sortResult+i) > *(sortResult+j)) {
				temp = *(sortResult+i);
				*(sortResult+i) = *(sortResult+j);
				*(sortResult+j) = temp;
			}
		}

	}
}
