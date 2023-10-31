#include "KeyBoard.h"
#include <Windows.h>


class KeyBoard {
public:
	KeyBoard() {
		HMIDIOUT handle;
		midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
	}
private:

};