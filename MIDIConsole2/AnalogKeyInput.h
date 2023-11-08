#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>

class AnalogKeyInput
{
public:
	static void runAnalogKeyInput();
private:
	static std::vector<int> input;

	static void detectKeyboardInput();
	static void printKey();
};

