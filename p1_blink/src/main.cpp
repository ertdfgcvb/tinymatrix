#include <Arduino.h>
#include "common/pico_driver_v5_pinout.h"

uint32_t frame = 0;

void setup() {
	pinMode(PICO_LED_PIN, OUTPUT);
}

void loop() {
	digitalWrite(PICO_LED_PIN, frame++ % 2);
	delay(100);
}
