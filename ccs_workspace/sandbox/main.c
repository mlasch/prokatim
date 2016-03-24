/*
 * main.c
 */

#include <sandboxcfg.h>
#include <csl.h>
#include <csl_irq.h>

#include <DSK6713.h>
#include <DSK6713_led.h>

#include "config_audio_codec.h"






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

void EDMA_service_routine() {
	cnt++;
}
