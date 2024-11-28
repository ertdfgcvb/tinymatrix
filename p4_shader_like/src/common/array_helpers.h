void shuffle(uint16_t array[], uint32_t len) {
	if (len > 1) {
		uint32_t i;
		for (i = 0; i < len - 1; i++) {
			uint32_t j = i + rand() / (RAND_MAX / (len - i) + 1);
			uint32_t t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}
}

void shuffle(uint8_t array[], uint32_t len) {
  if (len > 1) {
    uint32_t i;
    for (i = 0; i < len - 1; i++) {
      uint32_t j = i + rand() / (RAND_MAX / (len - i) + 1);
      uint32_t t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

uint16_t getMaxValue(uint8_t *arr, uint16_t len) {
	uint16_t maxVal = 0;
	for (uint16_t i = 0; i < len; i++) {
		if (arr[i] > maxVal) maxVal = arr[i];
	}
	return maxVal;
}