#pragma once

#include <string>
#include <iostream>
#include <Windows.h>

#include "Midi.h"
#include "Menu.h"


class TickMenu
{
public:
	static void setTick(int tick);
	static void runTickMenu();
	static void showTickMenu();
private:
	static int tick;
};

