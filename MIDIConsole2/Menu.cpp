#include "Menu.h"



bool Menu::flag = false;
int Menu::state = QUIT;
char Menu::input = 0;





// ���˵���ʽ����
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

// ͨ��state�ı�˵�״̬
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

// �趨�˵�Ŀǰ״̬
void Menu::setState(STATE state) {
	Menu::state = state;
}

// ̽���������input
void Menu::detectKeyboardInput() {
	Menu::input = 0;
	for (int key = 1; key < 256; key++) {
		// ̽����Ƿ񱻰���
		if (GetKeyState(key) & 0x8000) {
			Menu::input = key;
		}
	}

}

// ��ȡ��Ұ��µļ�
int Menu::getKey() {
	return Menu::input;
}



// ͨ��input�������˵�
void Menu::runMenu() {


	switch (Menu::input) {

	case VK_RETURN: // Enter�� ���������
		Menu::state = MIDI;
		break;
	case '0': // 0�� �˳�
		Menu::state = QUIT;
		Menu::quit();
		break;
	case VK_NUMPAD0:
		Menu::state = QUIT;
		Menu::quit();
		break;
	case 'S': // S�� ����
		Menu::state = SETTING;
		Menu::showSetting();
		break;
	}
}


// չʾ�˵�
void Menu::showMenu() {
	system("cls");
	std::cout << "MIDI�����ٲ˵�:" << std::endl;
	std::cout << "����Enter����������ٵ���,S:����,0:�˳�" << std::endl;
	std::cout << "�밴����İ���:";
}

// չʾ����
void Menu::showSetting() {
	system("cls");
	std::cout << "MIDI����������:" << std::endl;
	std::cout << "��������9�˳�" << std::endl;
	std::cout << "A:��ɫ,B:��� C:��ʽ" << std::endl;
	std::cout << "�밴����İ���:";
}

// ����������
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


// �˳���ʽ
void Menu::quit() {
	Menu::state = QUIT;
	Menu::flag = false;
}
