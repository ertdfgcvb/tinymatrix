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

#define SERIAL_SPEED 460800 // 230400 115200

#include "common/pico_driver_v5_pinout.h"
#include "common/font/Font_apple5x7.h"
#include "common/array_helpers.h"
#include "common/gfx_helpers.h"
#include "common/testcard.h"
#include "common/serial_loop.h"

SerialLoop sl;

const bitmap_font *FONT = &apple5x7;
MatrixPanel_I2S_DMA *display = nullptr;

uint32_t frame = 0;


void setup() {

	delay(1000);

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
	display->setBrightness(253); // caution: use 254 as max valuer
}


void loop() {
	display->clearScreen();
	for (uint8_t j=0; j<4; j++) {
		char s[2];
		s[1] = '\0';
		uint8_t offs = (sin(j * 0.05 + frame * 0.01) + 1) * 32;
		for (uint8_t i=0; i<14; i++) {
			s[0] = 32 + (i + frame + offs) % 95;
			u_int8_t r = (sin((j + i + frame) * 0.29) + 1) * 128;
			u_int8_t g = (sin((j + i + frame) * 0.31) + 1) * 128;
			u_int8_t b = (sin((j + i + frame) * 0.43) + 1) * 128;
			uint16_t col = MatrixPanel_I2S_DMA::color565(r, g, b);
			drawString(display, FONT->Width * i, (FONT->Height + 1) * j, FONT, col, s);
		}
	}
	display->flipDMABuffer();
	digitalWrite(PICO_LED_PIN, frame % 2);
	frame++;
	delay(30);
}
