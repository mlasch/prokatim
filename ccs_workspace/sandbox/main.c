/*
 * main.c
 * 		Author: mlasch@mailbox.org
 */

#include <sandboxcfg.h>
#include <csl.h>
#include <csl_irq.h>

#include <DSK6713.h>
#include <DSK6713_led.h>

#include "config_audio_codec.h"

int main(void) {
	CSL_init();

	DSK6713_init();
	DSK6713_LED_init();

	IRQ_globalDisable();

	DSK6713_configure_AIC23();

	IRQ_globalEnable();

	DSK6713_LED_off(0);
	DSK6713_LED_off(1);
	DSK6713_LED_off(2);
	DSK6713_LED_off(3);

	return 0;
}


