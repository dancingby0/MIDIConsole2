#pragma once
#include <string>
#include <iostream>
#include <Windows.h>

#include "Midi.h"
#include "Menu.h"

class TimbreMenu
{
public:
	static void runTimbre();
	static void showTimbre();

private:
	static int page;
	static int id_pointer;




	static void showPage();

};

