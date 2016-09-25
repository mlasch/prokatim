/*
 * display_task.c
 *
 *  Created on: 25.04.2016
 *      Author: marc
 */

#include <string.h>

#include <sandboxcfg.h>		// includes all the necessary stuff for dsp bios

#include <DSK6713.h>
#include <DSK6713_led.h>

#include "globals.h"

static void LED_out(char c) {
	switch(c) {
	case '1':
		DSK6713_LED_off(0);
		DSK6713_LED_off(1);
		DSK6713_LED_off(2);
		DSK6713_LED_on(3);
		break;

	case '2':
		DSK6713_LED_off(0);
		DSK6713_LED_off(1);
		DSK6713_LED_on(2);
		DSK6713_LED_off(3);
		break;

	case '3':
		DSK6713_LED_off(0);
		DSK6713_LED_off(1);
		DSK6713_LED_on(2);
		DSK6713_LED_on(3);
		break;

	case '4':
		DSK6713_LED_off(0);
		DSK6713_LED_on(1);
		DSK6713_LED_off(2);
		DSK6713_LED_off(3);
		break;

	case '5':
		DSK6713_LED_off(0);
		DSK6713_LED_on(1);
		DSK6713_LED_off(2);
		DSK6713_LED_on(3);
		break;

	case '6':
		DSK6713_LED_off(0);
		DSK6713_LED_on(1);
		DSK6713_LED_on(2);
		DSK6713_LED_off(3);
		break;

	case '7':
		DSK6713_LED_off(0);
		DSK6713_LED_on(1);
		DSK6713_LED_on(2);
		DSK6713_LED_on(3);
		break;

	case '8':
		DSK6713_LED_on(0);
		DSK6713_LED_off(1);
		DSK6713_LED_off(2);
		DSK6713_LED_off(3);
		break;

	case '9':
		DSK6713_LED_on(0);
		DSK6713_LED_off(1);
		DSK6713_LED_off(2);
		DSK6713_LED_on(3);
		break;

	case '*':
		DSK6713_LED_on(0);
		DSK6713_LED_off(1);
		DSK6713_LED_on(2);
		DSK6713_LED_off(3);
		break;

	case '0':
		DSK6713_LED_off(0);
		DSK6713_LED_off(1);
		DSK6713_LED_off(2);
		DSK6713_LED_off(3);
		break;

	case '#':
		DSK6713_LED_on(0);
		DSK6713_LED_off(1);
		DSK6713_LED_on(2);
		DSK6713_LED_on(3);
		break;

	default:
		DSK6713_LED_on(0);
		DSK6713_LED_on(1);
		DSK6713_LED_on(2);
		DSK6713_LED_on(3);
	}
}

void display_task() {
	//int i;
	while(1) {
		SEM_pendBinary(&SEM_NewChar, SYS_FOREVER);

		/*for (i = 0;i<strlen(gDigitList);i++) {
			LED_out(gDigitList[i]);
			TSK_sleep(5000);
		}

		LED_out('\0');*/
		LED_out(gNewDigit);
	}
}
