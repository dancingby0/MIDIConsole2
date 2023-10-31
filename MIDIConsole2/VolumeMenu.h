#pragma once
#include <iostream>
#include <Windows.h>

#include "Menu.h"
#include "Midi.h"

class VolumeMenu
{
public:
	static void runVolumeMenu();
	static void showVolumeMenu();
private:
	static int volume;
};