#pragma once
#ifndef TIMBREMENU_H
#define TIMBREMENU_H


#include <string>
#include <iostream>
#include <Windows.h>

#include "Midi.h"
#include "Menu.h"

class TimbreMenu {
public:
	static void runTimbre();
	static void showTimbre();
	static void setTimbre(int);

private:
	static int page;
	static int id_pointer;




	static void showPage();

};

#endif // !TIMBREMENU_H