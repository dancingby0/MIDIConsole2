#include "AnalogKeyInput.h"
std::vector<int> AnalogKeyInput::input;

void AnalogKeyInput::runAnalogKeyInput() {
	while (true) {

	}
}


void AnalogKeyInput::detectKeyboardInput(){

	AnalogKeyInput::input.clear();
	for (int key = 1; key < 256; key++) {
		// ̽����Ƿ񱻰���
		if (GetKeyState(key) & 0x8000) {
			AnalogKeyInput::input.push_back(key);
		}
	}
}