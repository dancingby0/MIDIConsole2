#include "Menu.h"


// Ĭ������
bool Menu::flag = false;
int Menu::state = QUIT;
char Menu::input = 0;

// ��ȡ��Ұ��µļ�
int Menu::getKey() {
	return Menu::input;
}

// ���˵���ʽ����
void Menu::start() {

	Menu::flag = true;
	Menu::state = MENU; // �˵�״̬����Ϊ"���˵�"
	Midi::initialMidi(); // ��ʼ��Midi������
	VolumeMenu::setVolume(Midi::getVolume());
	TimbreMenu::setTimbre(Midi::getTimbre());
	ModeMenu::setMode(Midi::getMode());
	TickMenu::setTick(Midi::getTick());
	Menu::showMenu();
	while (Menu::flag) {
		Menu::detectKeyboardInput(); // ̽���������
		Menu::changeMenu(); // ���Ĳ˵���״̬
		Sleep(10); // �趨��΢����ʱ
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

	case TICK:
		TickMenu::runTickMenu();
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
	std::cout << "A:��ɫ,B:��� C:��ʽ D:���浱ǰ���� E:ʱ���" << std::endl;
	std::cout << "�밴����İ���:";
}

void Menu::runSAVE() {
	Midi::writeConfigFile();
	
	// �Ե�ʽ�����ı�����
	std::string tmp;
	if (MidiConfig::getMode() < 7) {
		tmp = MidiConfig::getMode() + 'A';
		tmp += "���ʽ";
	}
	else {
		tmp = MidiConfig::getMode() - 7 + 'A';
		tmp += "С��ʽ";
	}
	
	system("cls");
	std::cout << "�ɹ����浱ǰ����Ϊ:" << std::endl;
	std::cout << "��ɫ : " << Midi::getInstrumentList()->at(Midi::getTimbre()).Chinese_name << std::endl;
	std::cout << "��� : " << Midi::getVolume() << std::endl;
	std::cout << "��ʽ : " << tmp << std::endl;
	std::cout << "ʱ��� : " << Midi::getTick() << std::endl;
	Sleep(2000);
	Menu::showSetting();
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

	case 'D':
		Menu::runSAVE();
		break;

	case 'E':
		Menu::state = TICK;
		TickMenu::showTickMenu();
		break;
	}

}

// �˳���ʽ
void Menu::quit() {
	Menu::state = QUIT;
	Menu::flag = false;
}
