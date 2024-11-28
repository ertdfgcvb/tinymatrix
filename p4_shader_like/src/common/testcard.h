#define COLOR_DELAY 2000

void RGB_test(MatrixPanel_I2S_DMA *display) {
	display->fillScreen(RED);
	display->flipDMABuffer();
	delay(COLOR_DELAY);

	display->fillScreen(GREEN);
	display->flipDMABuffer();
	delay(COLOR_DELAY);

	display->fillScreen(BLUE);
	display->flipDMABuffer();
	delay(COLOR_DELAY);

	display->fillScreen(WHITE);
	display->flipDMABuffer();
	delay(COLOR_DELAY);

	display->clearScreen();
	display->flipDMABuffer();
	delay(COLOR_DELAY);

	// for (uint8_t i=0; i<64; i++) {
	// 	layer->drawPixel(i, 10, rgb24(i, i, i));
	// 	layer->drawPixel(i, 14, {i, i, i});
	// }
	// layer->swapBuffers();
	// delay(100000);

	/*
	for (int i=0; i<10; i++) {
		bg.fillScreen(WHITE);
		bg.swapBuffers();
		delay(1000);
		for (int j=0; j<32; j++){
			bg.drawLine(0, j, 64, j, BLACK);
		}
		bg.swapBuffers();
		delay(1000);
	}

	bg.fillScreen(rgb24(0, 0, 0));

	// arrow (top, left)
	bg.drawLine(0, 0, 8, 0, WHITE);
	bg.drawLine(0, 0, 8, 8, WHITE);
	bg.drawLine(0, 0, 0, 8, WHITE);

	// dots
	bg.drawPixel(w - 1, 0, { 255, 0, 0 });
	bg.drawPixel(w - 1, h - 1, { 0, 255, 0 });
	bg.drawPixel(0, h - 1, { 0, 0, 255 });

	// center +
	bg.drawLine(w / 2 - 4, h / 2, w / 2 + 4, h / 2, WHITE);
	bg.drawLine(w / 2, h / 2 - 4, w / 2, h / 2 + 4, WHITE);
	bg.drawCircle(w / 2, h / 2, 8, { 5, 5, 5 });

	bg.swapBuffers();
	delay(2000);
	*/
}