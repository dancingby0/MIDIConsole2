#pragma once
#include <iostream>
#include <Windows.h>

#include "Menu.h"
#include "Midi.h"

class ModeMenu
{
public:

	static void showModeMenu();
	static void runModeMenu();
private:
	static int mode_pointer;

	static void showModeTable();

};

