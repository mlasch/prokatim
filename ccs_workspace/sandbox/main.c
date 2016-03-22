/*
 * main.c
 */

#include <sandboxcfg.h>
#include <csl.h>

#include <DSK6713.h>
#include <DSK6713_led.h>

int main(void) {
	CSL_init();

	DSK6713_init();
	DSK6713_LED_init();

	DSK6713_LED_on(0);
	DSK6713_LED_off(1);
	DSK6713_LED_on(2);
	DSK6713_LED_off(3);
	return 0;
}

void processPong() {
}
