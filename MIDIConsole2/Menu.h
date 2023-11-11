#pragma once
#ifndef MENU_H
#define MENU_H

#include <Windows.h>
#include <iostream>

#include "TimbreMenu.h"
#include "Midi.h"
#include "VolumeMenu.h"
#include "ModeMenu.h"
#include "TickMenu.h"
#include "ScoreMenu.h"

const enum STATE {
	QUIT, MENU, SETTING, TIMBRE, VOLUME, MODE, MIDI, SAVE, TICK, SCOREMENU
};

class Menu {
public:
	static void start();
	static void showSetting();
	static int getKey();
	static void setState(STATE);
	static void showMenu();

private:
	static bool flag; // ��־��ʽ������
	static char input; // ��ҵ�����
	static int state;// ��־��ʽ��״̬

	static void runSAVE();
	static void detectKeyboardInput();
	static void changeMenu();
	static void runMenu();
	static void runSetting();
	static void quit();
};

#endif // !MENU_H