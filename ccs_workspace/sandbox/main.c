/*
 * main.c
 */

#include <sandboxcfg.h>
#include <csl.h>
#include <csl_int.h>

#include <DSK6713.h>
#include <DSK6713_led.h>

#include "config_audio_codec.h"

//#pragma DATA_SECTION(buffer_IN_A, ".data");
//#pragma DATA_SECTION(buffer_IN_B, ".data");
//#pragma DATA_SECTION(buffer_OUT_A, ".databuffer");
//#pragma DATA_SECTION(buffer_OUT_B, ".databuffer");

Int32 buffer_IN_A[108];
//Int16 buffer_IN_B[108];
//Int16 buffer_OUT_A[108];
//Int16 buffer_OUT_B[108];

volatile Uint32 cnt;

int main(void) {
	cnt = 0;

	CSL_init();

	DSK6713_init();
	DSK6713_LED_init();


	DSK6713_configure_AIC23();

	IRQ_globalEnable();

	DSK6713_LED_on(0);
	DSK6713_LED_off(1);
	DSK6713_LED_on(2);
	DSK6713_LED_off(3);
	return 0;
}

void EDMA_RX_interrupt() {
	cnt++;
}
