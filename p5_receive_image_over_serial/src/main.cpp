#include <Arduino.h>
#include <Preferences.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

// Physical size of the panel
#define PANEL_WIDTH    64
#define PANEL_HEIGHT   32

// Number of chained panels
#define CHAIN_LENGHT    1
#define TOTAL_WIDTH    64
#define TOTAL_HEIGHT   32

const uint16_t NUM_LEDS = TOTAL_WIDTH * TOTAL_HEIGHT;
const uint16_t BUFFER_SIZE = NUM_LEDS * 3; // Assume RGB data
uint8_t buf[BUFFER_SIZE];                  // A buffer for the incoming data

#define SERIAL_SPEED 2000000 // 2000000 921600 460800 230400 115200

#include "common/font/Font_apple5x7.h"
#include "common/pico_driver_v5_pinout.h"
#include "common/array_helpers.h"
#include "common/gfx_helpers.h"
#include "common/testcard.h"

const bitmap_font *FONT = &apple5x7;
MatrixPanel_I2S_DMA *display = nullptr;

int frame = 0;

void setup() {

	Serial.begin(SERIAL_SPEED);
	Serial.setTimeout(50);

	pinMode(PICO_LED_PIN, OUTPUT);

	HUB75_I2S_CFG mxconfig;
	mxconfig.gpio = {
		R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN,
		A_PIN, B_PIN, C_PIN, D_PIN, E_PIN,
		LAT_PIN, OE_PIN, CLK_PIN
	};
	mxconfig.mx_width = PANEL_WIDTH;
	mxconfig.mx_height = PANEL_HEIGHT;
  	mxconfig.chain_length = CHAIN_LENGHT;
	mxconfig.double_buff = true;
	mxconfig.clkphase = false;
	mxconfig.driver = HUB75_I2S_CFG::FM6126A;
	mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_20M;
	mxconfig.min_refresh_rate = 120;
	mxconfig.latch_blanking = 1;

	display = new MatrixPanel_I2S_DMA(mxconfig);

	if (not display->begin()) {
		while(true) {
			digitalWrite(PICO_LED_PIN, 1);
			delay(50);
			digitalWrite(PICO_LED_PIN, 0);
			delay(50);
		}
	}

	// display->setBrightness(50); // caution, reduce brightness to avoid overload
	// RGB_test(display);
	display->setBrightness(253); // caution: use 253 as max value
}

void loop() {

	char chr = Serial.read();
	if (chr == '*') {  // Special char to signal incoming data
		uint16_t count = Serial.readBytes((char *)buf, BUFFER_SIZE);
		//if (count == BUFFER_SIZE) {
			display->clearScreen();
			uint16_t idx = 0;
			for (uint16_t i = 0; i < NUM_LEDS; i++) {
				uint8_t x = i % TOTAL_WIDTH;
				uint8_t y = i / TOTAL_WIDTH;
				uint8_t r = buf[idx++];
				uint8_t g = buf[idx++];
				uint8_t b = buf[idx++];
				display->drawPixelRGB888(x, y, r, g, b);
			}
			display->flipDMABuffer();
		//}
	} else {
		Serial.flush();
		/*
		display->clearScreen();

		// Info (frame counter)
		char out[16] = "";
		sprintf(out, "%s%d", "RX:", Serial.available());
		drawString(display, 0, 0, FONT, WHITE, out);
		display->drawPixelRGB888(TOTAL_WIDTH-1, 0, frame/20 % 2 * 255,0,0);
		display->flipDMABuffer();
		*/
	}
	digitalWrite(PICO_LED_PIN, frame / 20 % 2);
	frame++;
	delay(1);
}
