#pragma once
#ifndef VOLUMEMENU_H
#define VOLUMEMENU_H

#include <iostream>
#include <Windows.h>

#include "Menu.h"
#include "Midi.h"

class VolumeMenu {
public:
	static void setVolume(int volume);
	static void runVolumeMenu();
	static void showVolumeMenu();
private:
	static int volume;
};

#endif // !VOLUMEMENU_H