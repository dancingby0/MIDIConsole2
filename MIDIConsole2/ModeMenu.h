#pragma once

#ifndef MODEMENU_H
#define MODEMENU_H



#include <iostream>
#include <Windows.h>

#include "Menu.h"
#include "Midi.h"

class ModeMenu {
public:

	static void setMode(int);
	static void showModeMenu();
	static void runModeMenu();
private:
	static int mode_pointer;

	static void showModeTable();

};

#endif // !MODEMENU_H