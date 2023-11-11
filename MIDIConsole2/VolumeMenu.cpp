#include "VolumeMenu.h"


int VolumeMenu::volume;

void VolumeMenu::setVolume(int volume) {
	VolumeMenu::volume = volume;
}
// 响度菜单主程式
void VolumeMenu::runVolumeMenu() {
	switch (Menu::getKey()) {
		// 退出
	case 'Q':
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
		// 保存
	case VK_RETURN:
		Midi::setVolume(VolumeMenu::volume);
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
		// 向上选择
	case VK_UP:
		if (VolumeMenu::volume < 126) {
			VolumeMenu::volume++;
			VolumeMenu::showVolumeMenu();
			Sleep(10);
		}
		break;
		// 向下选择
	case VK_DOWN:
		if (VolumeMenu::volume > 0) {
			VolumeMenu::volume--;
			VolumeMenu::showVolumeMenu();
			Sleep(10);
		}
		break;
	}
}

// 显示响度菜单
void VolumeMenu::showVolumeMenu() {
	system("cls");
	std::cout << "MIDI电子琴设置:" << std::endl;
	std::cout << "按q退出 按↑提高音量 按↓降低音量 按Enter保存音量设置" << std::endl;
	std::cout << "目前音量为:" << VolumeMenu::volume << std::endl;
}