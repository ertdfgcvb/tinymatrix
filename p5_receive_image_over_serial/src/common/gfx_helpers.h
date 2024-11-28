// Pass 8-bit (each) R,G,B, get back 16-bit packed colour
// https://github.com/squix78/ILI9341Buffer/blob/master/ILI9341_SPI.cpp

/*inline*/ uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

#define MAX_GRAY_VALUE 254 // 255 will burn grays
#define MIN_GRAY_VALUE 16

// with color correction enabled
uint16_t WHITE = color565(255, 255, 255);
uint16_t BLACK = color565(0, 0, 0);
uint16_t GRAY  = color565(99, 99, 99);
uint16_t RED   = color565(255, 0, 0);
uint16_t GREEN = color565(0, 255, 0);
uint16_t BLUE  = color565(0, 0, 255);


void drawPixel(MatrixPanel_I2S_DMA *display, int16_t x, int16_t y,  uint16_t color) {
	if (x >= PANEL_WIDTH) return;
	if (y >= PANEL_HEIGHT) {
		x = x + y / PANEL_HEIGHT * PANEL_WIDTH;
		y = y % PANEL_HEIGHT;
	}
	display->drawPixel(x, y, color);
}

void drawString(MatrixPanel_I2S_DMA *display, int16_t x, int16_t y,  const bitmap_font *font, uint16_t charColor, const char text[]) {
    int xcnt, ycnt, offset = 0;
    char character;
    while ((character = text[offset++]) != '\0') {
        for (ycnt = 0; ycnt < font->Height; ycnt++) {
            for (xcnt = 0; xcnt < font->Width; xcnt++) {
                if (getBitmapFontPixelAtXY(character, xcnt, ycnt, font)) {
                    // display->drawPixel(x + xcnt, y + ycnt, charColor);
					drawPixel(display, x + xcnt, y + ycnt, charColor);
                }
            }
        }
        x += font->Width;
    }
}

void value_palette(uint16_t palette[], uint16_t palette_len, uint8_t mode) {
	// Constant
	if (mode == 0) {
		for (uint8_t i=0; i<palette_len; i++) {
			palette[i] = color565(255, 255, 255);
		}
	}
	// Linear / Saw
	else if (mode == 1) {
		for (uint8_t i=0; i<palette_len; i++) {
			double t = (double) i / (palette_len-1);
			uint8_t gray = uint8_t (t * (MAX_GRAY_VALUE - MIN_GRAY_VALUE) + MIN_GRAY_VALUE);
			palette[i] = color565(gray, gray, gray);
		}

	}
	// Triangle
	else if (mode == 2) {
		for (uint8_t i=0; i<palette_len; i++) {
			double t = (double) i / (palette_len-1);
			t = abs(t - 0.5) * 2.0;
			uint8_t gray = uint8_t (t * (MAX_GRAY_VALUE - MIN_GRAY_VALUE) + MIN_GRAY_VALUE);
			palette[i] = color565(gray, gray, gray);
		}
	}
}

void random_palette(uint16_t palette[], uint16_t palette_len) {

	// randomSeed(seed);

	double rA = 0.51;
	double gA = 0.51;
	double bA = 0.51;
	double rB = 0.49;
	double gB = 0.49;
	double bB = 0.49;
	double rC = (double) rand() / RAND_MAX;
	double gC = (double) rand() / RAND_MAX;
	double bC = (double) rand() / RAND_MAX;
	double rD = (double) rand() / RAND_MAX;
	double gD = (double) rand() / RAND_MAX;
	double bD = (double) rand() / RAND_MAX;

	for (uint8_t i=0; i<palette_len; i++) {
		double t = (double) i / palette_len;
		uint8_t r = (rA + rB * cos(6.28318530718 * rC * t + rD)) * (255-MIN_GRAY_VALUE) + MIN_GRAY_VALUE;
		uint8_t g = (gA + gB * cos(6.28318530718 * gC * t + gD)) * (255-MIN_GRAY_VALUE) + MIN_GRAY_VALUE;
		uint8_t b = (bA + bB * cos(6.28318530718 * bC * t + bD)) * (255-MIN_GRAY_VALUE) + MIN_GRAY_VALUE;
		palette[i] = color565(r, g, b);
	}
}