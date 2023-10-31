#include "ModeMenu.h"
#include "Menu.h"
#include "Midi.h"
#include <iostream>
#include <Windows.h>


int ModeMenu::mode_pointer = Midi::getMode();

void ModeMenu::showModeMenu() {
	system("cls");
	std::cout << "��ʽ����:" << std::endl;
	std::cout << "��q����,��Enter����,������ѡ��,������ѡ��" << std::endl;
	showModeTable();
}

void ModeMenu::showModeTable() {
	for (int i : *Midi::getModeList()) {
		if (i == ModeMenu::mode_pointer) std::cout << "->   ";
		if (0 <= i && i <= 6) {

			std::cout << static_cast<char>(i + 'A') << "���" << std::endl;
		}
		if (7 <= i && i <= 13) {

			std::cout << static_cast<char>(i - 7 + 'A') << "С��" << std::endl;
		}
	}
}
void ModeMenu::runModeMenu() {
	switch (Menu::getKey()) {
	case 'Q':
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
	case VK_UP:
		if (ModeMenu::mode_pointer >= 1) {
			ModeMenu::mode_pointer--;
			ModeMenu::showModeMenu();
			Sleep(100);
		}
		break;

	case VK_DOWN:
		if (ModeMenu::mode_pointer <= 12) {
			ModeMenu::mode_pointer++;
			ModeMenu::showModeMenu();
			Sleep(100);
		}
		break;

	case VK_RETURN:
		Midi::setMode(mode_pointer);
		Menu::setState(SETTING);
		Menu::showSetting();
		break;

	default:
		break;
	}
}