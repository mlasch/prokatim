/*
 * config_audio_codec.h
 *
 *  Created on: 23.03.2016
 *      Author: marc
 */

#ifndef CONFIG_AUDIO_CODEC_H_
#define CONFIG_AUDIO_CODEC_H_

/*
 * aic23 register map
 */
#define LEFT_LINE_INPUT_CHANNEL_VOLUME		0
#define RIGHT_LINE_INPUT_CHANNEL_VOLUME		1
#define LEFT_CHANNEL_HEADPHONE_VOLUME		2
#define RIGHT_CHANNEL_HEADPHONE_VOLUME		3
#define ANALOG_AUDIO_PATH					4
#define DIGITAL_AUDIO_PATH					5
#define POWER_DOWN_CONTROL					6
#define DIGITAL_AUDIO_INTERFACE_FORMAT		7
#define SAMPLE_RATE_CONTROL					8
#define DIGITAL_INTERFACE_ACTIVATION		9
#define RESET_REGISTER						15

#define BUFFER_SIZE 108;
#define SAMPLE_RATE 8000;

enum {
	StateA, StateB
};

typedef struct {
	Uint32 *cpuRcvBufferPtr;
	Uint32 *cpuXmtBufferPtr;
	Uint32 cpuBufferState;
} BufferStateTypeDef;

void DSK6713_configure_AIC23(void);

#endif /* CONFIG_AUDIO_CODEC_H_ */
