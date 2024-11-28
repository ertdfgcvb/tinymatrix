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
bool grid[PANEL_HEIGHT][PANEL_WIDTH];         // Current grid
bool newGrid[PANEL_HEIGHT][PANEL_WIDTH];      // Updated grid
uint32_t seconds_counter = 0;

// Initialize grid with a random pattern
void initializeGrid() {
  for (int i = 0; i < PANEL_HEIGHT; i++) {
    for (int j = 0; j < PANEL_WIDTH; j++) {
      grid[i][j] = random(2); // 0 or 1
    }
  }
}

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
	display->clearScreen();
	initializeGrid();
}



int countNeighbors(int x, int y) {
  int count = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (i == 0 && j == 0) continue; // Skip the cell itself
      int nx = (x + i + PANEL_HEIGHT) % PANEL_HEIGHT; // Wrap around edges
      int ny = (y + j + PANEL_WIDTH) % PANEL_WIDTH;
      count += grid[nx][ny];
    }
  }
  return count;
}

void updateGrid() {
  for (int i = 0; i < PANEL_HEIGHT; i++) {
    for (int j = 0; j < PANEL_WIDTH; j++) {
      int neighbors = countNeighbors(i, j);
      if (grid[i][j] == 1) {
        // Live cell: survives with 2 or 3 neighbors
        newGrid[i][j] = (neighbors == 2 || neighbors == 3);
      } else {
        // Dead cell: becomes alive with exactly 3 neighbors
        newGrid[i][j] = (neighbors == 3);
      }
    }
  }
  // Copy newGrid to grid
  for (int i = 0; i < PANEL_HEIGHT; i++) {
    for (int j = 0; j < PANEL_WIDTH; j++) {
      grid[i][j] = newGrid[i][j];
    }
  }
}

void displayGrid() {
  display->clearScreen();
  float now = frame * 0.04;
  for (int i = 0; i < PANEL_HEIGHT; i++) {
    for (int j = 0; j < PANEL_WIDTH; j++) {

      float px = i * 0.08 - 0.01;
			float py = j * 0.08;

			float color = 0.0;
			color += sin(px - py) ;
			color += sin(now)* cos(sin(now)*py*px*sin(px))+.008;
			color += sin(now)+px*sin(py*sin(sin(tan(cos (now)))));

			uint8_t r = 128.0 +	sin(color*color)*127.0*sin(now+px/(now*3.14));
			uint8_t g =	128.0 +	sin(color*color)*127.0;
			uint8_t b =	128.0 +	cos(color * 0.333 + 1.3)*128.0;
	    display->drawPixelRGB888(j, i, grid[i][j] * r, grid[i][j] * g, grid[i][j] * b);
    }
  }
  frame++;
  display->flipDMABuffer();
}

void loop() {
  displayGrid();
  updateGrid();
  delay(100); // Delay between generations
  seconds_counter++;
  if (seconds_counter % 300 == 0)
  {
	initializeGrid();
  }
}
