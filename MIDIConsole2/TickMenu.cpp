#include "TickMenu.h"



int TickMenu::tick;

void TickMenu::setTick(int tick) {
	TickMenu::tick = tick;
}
// ��Ȳ˵�����ʽ
void TickMenu::runTickMenu() {
	switch (Menu::getKey()) {
		// �˳�
	case 'Q':
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
		// ����
	case VK_RETURN:
		Midi::setTick(TickMenu::tick);
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
		// ����ѡ��
	case VK_UP:
		if (TickMenu::tick < 999) {
			TickMenu::tick++;
			TickMenu::showTickMenu();
			Sleep(10);
		}
		break;
		// ����ѡ��
	case VK_DOWN:
		if (TickMenu::tick > 0) {
			TickMenu::tick--;
			TickMenu::showTickMenu();
			Sleep(10);
		}
		break;
	}
}

// ��ʾ��Ȳ˵�
void TickMenu::showTickMenu() {
	system("cls");
	std::cout << "MIDI����������:" << std::endl;
	std::cout << "��q�˳� �������ʱ��� ��������ʱ��� ��Enter����ʱ�������" << std::endl;
	std::cout << "Ŀǰʱ���Ϊ(��λ:ms):" << TickMenu::tick << std::endl;
}