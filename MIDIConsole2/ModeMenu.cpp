#include "ModeMenu.h"

// 获取调式的菜单指向位置
int ModeMenu::mode_pointer = Midi::getMode();

// 根据玩家输入更改调式
void ModeMenu::runModeMenu() {
	switch (Menu::getKey()) {
	// 退出
	case 'Q':
		Menu::setState(SETTING);
		Menu::showSetting();
		break;

	// 调式菜单向上选择
	case VK_UP:
		if (ModeMenu::mode_pointer >= 1) {
			ModeMenu::mode_pointer--;
			ModeMenu::showModeMenu();
			Sleep(150); // 选定后延迟100ms
		}
		break;

	// 调式菜单向下选择
	case VK_DOWN:
		if (ModeMenu::mode_pointer <= 12) {
			ModeMenu::mode_pointer++;
			ModeMenu::showModeMenu();
			Sleep(150);
		}
		break;

	// 保存设置
	case VK_RETURN:
		Midi::setMode(mode_pointer);
		Menu::setState(SETTING);
		Menu::showSetting();
		break;

	default:
		break;
	}
}

// 展示调式菜单
void ModeMenu::showModeMenu() {
	system("cls");
	std::cout << "调式设置:" << std::endl;
	std::cout << "按q返回,按Enter保存,↑向上选择,↓向下选择" << std::endl;
	showModeTable();
}

// 展示调式的内容
void ModeMenu::showModeTable() {
	for (int i : *Midi::getModeList()) {
		if (i == ModeMenu::mode_pointer) std::cout << "->   ";
		if (0 <= i && i <= 6) {
			std::cout << static_cast<char>(i + 'A') << "大调" << std::endl;
		}
		if (7 <= i && i <= 13) {
			std::cout << static_cast<char>(i - 7 + 'A') << "小调" << std::endl;
		}
	}
}