/*
 * dsp_goertzel.c
 *
 *  Created on: 30.03.2016
 *      Author: marc
 */

#include <csl.h>

#include <c67fastMath.h>

float goertzel(Int16* samples, Uint16 length, Uint32 sample_rate, Uint32 freq) {
	Uint16 i;
	float s_curr = 0, s_prev1 = 0, s_prev2 = 0;
	float x;

	const float pi = 3.1415;
	float m = (float)length * (float)freq / (float)sample_rate;
	float cos_const = cossp(2*pi*m/(float)length);

	float coeff = 2*cos_const;

	for (i=0;i<length;i++) {
		s_curr = samples[i] + coeff * s_prev1 - s_prev2;

		s_prev2 = s_prev1;
		s_prev1 = s_curr;
	}

	/* calculate the power */

	x = s_prev2*s_prev2 + s_prev1*s_prev1 - coeff*s_prev1*s_prev2;

	return x;
}
