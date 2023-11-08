#include "TickMenu.h"



int TickMenu::tick;

void TickMenu::setTick(int tick) {
	TickMenu::tick = tick;
}
// 响度菜单主程式
void TickMenu::runTickMenu() {
	switch (Menu::getKey()) {
		// 退出
	case 'Q':
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
		// 保存
	case VK_RETURN:
		Midi::setTick(TickMenu::tick);
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
		// 向上选择
	case VK_UP:
		if (TickMenu::tick < 999) {
			TickMenu::tick++;
			TickMenu::showTickMenu();
			Sleep(10);
		}
		break;
		// 向下选择
	case VK_DOWN:
		if (TickMenu::tick > 0) {
			TickMenu::tick--;
			TickMenu::showTickMenu();
			Sleep(10);
		}
		break;
	}
}

// 显示响度菜单
void TickMenu::showTickMenu() {
	system("cls");
	std::cout << "MIDI电子琴设置:" << std::endl;
	std::cout << "按q退出 按↑提高时间刻 按↓降低时间刻 按Enter保存时间刻设置" << std::endl;
	std::cout << "目前时间刻为(单位:ms):" << TickMenu::tick << std::endl;
}