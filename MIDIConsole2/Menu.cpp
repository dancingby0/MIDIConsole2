#include "Menu.h"



bool Menu::flag = false;
int Menu::state = QUIT;
char Menu::input = 0;





// 主菜单程式运行
void Menu::start() {

	Menu::flag = true;
	Menu::state = MENU;
	Midi::initialMidi();
	Menu::showMenu();
	while (Menu::flag) {
		Menu::changeMenu();
		Menu::detectKeyboardInput();
	}
}

// 通过state改变菜单状态
void Menu::changeMenu() {
	switch (Menu::state) {

	case MENU:
		Menu::runMenu();
		break;

	case SETTING:
		Menu::runSetting();
		break;

	case TIMBRE:
		TimbreMenu::runTimbre();
		break;

	case VOLUME:
		VolumeMenu::runVolumeMenu();
		break;

	case MODE:
		ModeMenu::runModeMenu();
		break;

	case MIDI:
		Midi::runMIDI();
		break;

	}
}

// 设定菜单目前状态
void Menu::setState(STATE state) {
	Menu::state = state;
}

// 探测键并传给input
void Menu::detectKeyboardInput() {
	Menu::input = 0;
	for (int key = 1; key < 256; key++) {
		// 探测键是否被按下
		if (GetKeyState(key) & 0x8000) {
			Menu::input = key;
		}
	}

}

// 获取玩家按下的键
int Menu::getKey() {
	return Menu::input;
}



// 通过input操作主菜单
void Menu::runMenu() {


	switch (Menu::input) {

	case VK_RETURN: // Enter键 进入电子琴
		Menu::state = MIDI;
		break;
	case '0': // 0键 退出
		Menu::state = QUIT;
		Menu::quit();
		break;
	case VK_NUMPAD0:
		Menu::state = QUIT;
		Menu::quit();
		break;
	case 'S': // S键 设置
		Menu::state = SETTING;
		Menu::showSetting();
		break;
	}
}


// 展示菜单
void Menu::showMenu() {
	system("cls");
	std::cout << "MIDI电子琴菜单:" << std::endl;
	std::cout << "按下Enter键进入电子琴弹奏,S:设置,0:退出" << std::endl;
	std::cout << "请按下你的按键:";
}

// 展示设置
void Menu::showSetting() {
	system("cls");
	std::cout << "MIDI电子琴设置:" << std::endl;
	std::cout << "按下数字9退出" << std::endl;
	std::cout << "A:音色,B:响度 C:调式" << std::endl;
	std::cout << "请按下你的按键:";
}

// 运行主设置
void Menu::runSetting() {
	switch (Menu::input) {
	case '9':
		Menu::state = MENU;
		Menu::showMenu();
		break;
	case VK_NUMPAD9:
		Menu::state = MENU;
		Menu::showMenu();
		break;
	case 'A':
		Menu::state = TIMBRE;
		TimbreMenu::showTimbre();
		break;
	case 'B':
		Menu::state = VOLUME;
		VolumeMenu::showVolumeMenu();
		break;
	case 'C':
		Menu::state = MODE;
		ModeMenu::showModeMenu();
		break;
	}

}


// 退出程式
void Menu::quit() {
	Menu::state = QUIT;
	Menu::flag = false;
}
