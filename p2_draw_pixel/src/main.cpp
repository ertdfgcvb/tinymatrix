#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "common/pico_driver_v5_pinout.h"

// Physical size of the panel
#define PANEL_WIDTH    64
#define PANEL_HEIGHT   32

// Number of chained panels
#define CHAIN_LENGHT    1
#define TOTAL_WIDTH    64
#define TOTAL_HEIGHT   32

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

	display->setBrightness(255); // range is 0-255, 0 - 0%, 255 - 100%

}

void loop() {
	display->clearScreen();
	display->drawPixelRGB888(10, 10, 255, 0, 0);
	display->flipDMABuffer();
	digitalWrite(PICO_LED_PIN, frame % 2);
	frame++;
	delay(10);
}
