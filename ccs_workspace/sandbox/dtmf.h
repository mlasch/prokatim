/*
 * dtmf.h
 *
 *  Created on: 07.04.2016
 *      Author: marc
 */

#ifndef DTMF_H_
#define DTMF_H_

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

extern dtmfValueTypeDef dtmfValue;

#endif /* DTMF_H_ */
