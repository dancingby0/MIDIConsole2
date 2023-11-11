#include "VolumeMenu.h"


int VolumeMenu::volume;

void VolumeMenu::setVolume(int volume) {
	VolumeMenu::volume = volume;
}
// ��Ȳ˵�����ʽ
void VolumeMenu::runVolumeMenu() {
	switch (Menu::getKey()) {
		// �˳�
	case 'Q':
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
		// ����
	case VK_RETURN:
		Midi::setVolume(VolumeMenu::volume);
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
		// ����ѡ��
	case VK_UP:
		if (VolumeMenu::volume < 126) {
			VolumeMenu::volume++;
			VolumeMenu::showVolumeMenu();
			Sleep(10);
		}
		break;
		// ����ѡ��
	case VK_DOWN:
		if (VolumeMenu::volume > 0) {
			VolumeMenu::volume--;
			VolumeMenu::showVolumeMenu();
			Sleep(10);
		}
		break;
	}
}

// ��ʾ��Ȳ˵�
void VolumeMenu::showVolumeMenu() {
	system("cls");
	std::cout << "MIDI����������:" << std::endl;
	std::cout << "��q�˳� ����������� ������������ ��Enter������������" << std::endl;
	std::cout << "Ŀǰ����Ϊ:" << VolumeMenu::volume << std::endl;
}