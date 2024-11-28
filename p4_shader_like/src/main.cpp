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

#include "common/font/Font_apple5x7.h"
#include "common/pico_driver_v5_pinout.h"
#include "common/array_helpers.h"
#include "common/gfx_helpers.h"
#include "common/testcard.h"

const bitmap_font *FONT = &apple5x7;
MatrixPanel_I2S_DMA *display = nullptr;

int frame = 0;

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
	display->setBrightness(253); // caution: use 253 as max value
}


void loop() {
	display->clearScreen();
	// adapted from https://www.shadertoy.com/view/XdX3Wn
	float now = frame * 0.04;
	float an= sin(now)/3.14157;
	float as= sin(an);
	float zoo = .23232+.38*sin(.7*now);

	for (uint8_t y=0; y<TOTAL_HEIGHT; y++) {
		for (uint8_t x=0; x<TOTAL_WIDTH; x++) {
			
			float px = x * 0.08 - 0.01;
			float py = y * 0.08;
			
			float color = 0.0;
			color += sin(px - py) ;
			color += sin(now)* cos(sin(now)*py*px*sin(px))+.008;
			color += sin(now)+px*sin(py*sin(sin(tan(cos (now)))));
			
			uint8_t r = 128.0 +	sin(color*color)*127.0*sin(now+px/(now*3.14));
			uint8_t g =	128.0 +	sin(color*color)*127.0;
			uint8_t b =	128.0 +	cos(color * 0.333 + 1.3)*128.0;

			// uint8_t r = 128.0 + (128.0 * sin((x / 4.0) - cos(now / 2) ));
			// uint8_t g = 128.0 + (128.0 * sin((y / 8.0) - sin(now) * 2 ));
			// uint8_t b = 128.0 + (128.0 * sin(sqrt((x - TOTAL_WIDTH / 2.0) * (x - TOTAL_WIDTH / 2.0) + (y - TOTAL_HEIGHT / 2.0) * (y - TOTAL_HEIGHT / 2.0)) / 4.0));

			display->drawPixelRGB888(x, y, r, g, b);
		}
	}

	char out[16] = "";

	sprintf(out, "%s%d", "FRM:", frame);

	drawString(display, 1, 1, FONT, BLACK, out);
	drawString(display, 0, 0, FONT, WHITE, out);
	display->flipDMABuffer();
	digitalWrite(PICO_LED_PIN, frame % 2);
	frame++;
	delay(1);
}
