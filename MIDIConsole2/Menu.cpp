#include "Menu.h"


// 默认设置
bool Menu::flag = false;
int Menu::state = QUIT;
char Menu::input = 0;

// 获取玩家按下的键
int Menu::getKey() {
	return Menu::input;
}

// 主菜单程式运行
void Menu::start() {

	Menu::flag = true;
	Menu::state = MENU; // 菜单状态设置为"主菜单"
	Midi::initialMidi(); // 初始化Midi电子琴
	VolumeMenu::setVolume(Midi::getVolume());
	TimbreMenu::setTimbre(Midi::getTimbre());
	ModeMenu::setMode(Midi::getMode());
	TickMenu::setTick(Midi::getTick());
	Menu::showMenu();
	while (Menu::flag) {
		Menu::detectKeyboardInput(); // 探测键的输入
		Menu::changeMenu(); // 更改菜单的状态
		Sleep(10); // 设定略微的延时
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

	case TICK:
		TickMenu::runTickMenu();
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
	std::cout << "A:音色,B:响度 C:调式 D:保存当前设置 E:时间刻" << std::endl;
	std::cout << "请按下你的按键:";
}

void Menu::runSAVE() {
	Midi::writeConfigFile();
	
	// 对调式进行文本推理
	std::string tmp;
	if (MidiConfig::getMode() < 7) {
		tmp = MidiConfig::getMode() + 'A';
		tmp += "大调式";
	}
	else {
		tmp = MidiConfig::getMode() - 7 + 'A';
		tmp += "小调式";
	}
	
	system("cls");
	std::cout << "成功保存当前设置为:" << std::endl;
	std::cout << "音色 : " << Midi::getInstrumentList()->at(Midi::getTimbre()).Chinese_name << std::endl;
	std::cout << "响度 : " << Midi::getVolume() << std::endl;
	std::cout << "调式 : " << tmp << std::endl;
	std::cout << "时间刻 : " << Midi::getTick() << std::endl;
	Sleep(2000);
	Menu::showSetting();
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

	case 'D':
		Menu::runSAVE();
		break;

	case 'E':
		Menu::state = TICK;
		TickMenu::showTickMenu();
		break;
	}

}

// 退出程式
void Menu::quit() {
	Menu::state = QUIT;
	Menu::flag = false;
}
