/*
 * main.c
 * 		Author: mlasch@mailbox.org
 */

#include <sandboxcfg.h>		// includes all the necessary stuff for dsp bios
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

	/* switch all leds off */
	DSK6713_LED_on(0);
	DSK6713_LED_on(1);
	DSK6713_LED_on(2);
	DSK6713_LED_on(3);

	return 0;
}
