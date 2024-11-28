#include <Arduino.h>

/**
AGYS
Helper to parse incoming messages from the serial port.
Serial messages are in the form of:
<label [val1] [val2]>

SerialLoop sl;
void loop(){
	sl.read(&Serial);
	if (sl.is_receiving()) {
		digitalWrite(LED_PIN, 1);
	} else {
		digitalWrite(LED_PIN, 0);
	}
	if (sl.has_data()) {
		String label = sl.get_label();
		uint8_t a = sl.get_a();
		uint8_t b = sl.get_b();
		...
	}
}

*/

class SerialLoop {

	// --------------------------------------------
	public:

	bool is_receiving() {
		return receive_in_progress;
	}

	String get_label() {
		return String(label);
	}

	uint8_t get_a() {
		return val_1;
	}

	uint8_t get_b() {
		return val_2;
	}

	bool has_data() {
		if (new_data) {
		new_data = false;
		return true;
		}
		return false;
	}

	void read(HardwareSerial *s) {

		static char rc;
		static uint8_t idx = 0;

		while (s->available() > 0) {
		rc = s->read();

		if (receive_in_progress == true) {
			if (rc != END_MARKER) {
			serial_buf[idx] = rc;
			idx++;
			if (idx >= BUF_SIZE) {
				idx = BUF_SIZE - 1;
			}
			} else {
			serial_buf[idx] = '\0';
			receive_in_progress = false;
			idx = 0;
			parse_data(serial_buf);
			new_data = true;
			//digitalWrite(LED_PIN, 0);
			}
		} else if (rc == START_MARKER) {
			receive_in_progress = true;
			//digitalWrite(LED_PIN, 1);
		}
		}
	}

	// --------------------------------------------
	private:

	static const uint8_t BUF_SIZE = 32;
	static const char START_MARKER = '<';
	static const char END_MARKER = '>';

	char label[BUF_SIZE] = {0}; // Label buffer (for parsing)
	uint8_t val_1 = 255;        // return value "a"
	uint8_t val_2 = 255;		// return value "b"

	bool receive_in_progress = false;
	bool new_data = false;

	char serial_buf[BUF_SIZE] = {0}; // Serial input buffer

	// Compare two arrays
	bool compare(char a[], char b[]) {
		for (int i = 0; a[i] != '\0'; i++) {
		if (a[i] != b[i]) return false;
		}
		return true;
	}

	// Parse the serial data
	void parse_data(char* buf) {

		char * str_token_idx;	// this is used by strtok() as an index

		// label, can be a single char or a sring
		str_token_idx = strtok(buf, " ");
		if (str_token_idx != NULL) {
		strcpy(label, str_token_idx);
		}

		// val 1
		str_token_idx = strtok(NULL, " ");
		if (str_token_idx != NULL) {
		val_1 = atoi(str_token_idx);
		}

		// val 2
		str_token_idx = strtok(NULL, " ");
		if (str_token_idx != NULL) {
		val_2 = atoi(str_token_idx);
		}
	}
};
