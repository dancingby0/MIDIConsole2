#include "TimbreMenu.h"



// 指明当前音色表的页数
int TimbreMenu::page;
int TimbreMenu::id_pointer;

// 音色菜单主程式
void TimbreMenu::runTimbre() {
	
	switch (Menu::getKey()){
	// 退出
	case 'Q':
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
	// 向左翻页音色表
	case VK_LEFT:
		if (TimbreMenu::page > 0) {
			TimbreMenu::page--;
			TimbreMenu::id_pointer = TimbreMenu::page * 20;
			TimbreMenu::showTimbre();
			Sleep(100);
		};
		break;
	// 向右翻页音色表
	case VK_RIGHT:
		if (TimbreMenu::page < 5) {
			TimbreMenu::page++;
			TimbreMenu::id_pointer = TimbreMenu::page * 20;
			TimbreMenu::showTimbre();
			Sleep(100);
		};
		break;
	// 向上选择音色
	case VK_UP:
		if (TimbreMenu::id_pointer > TimbreMenu::page * 20) {
			TimbreMenu::id_pointer--;
			TimbreMenu::showTimbre();
			Sleep(50);
		}
		break;
	// 向下选择音色
	case VK_DOWN:
		if (TimbreMenu::id_pointer < TimbreMenu::page * 20 + 19 and TimbreMenu::id_pointer < Midi::getInstrumentList()->size() - 1) {
			TimbreMenu::id_pointer++;
			TimbreMenu::showTimbre();
			Sleep(50);
		}
		break;
	// 保存设置
	case VK_RETURN:
		Midi::setTimbre(TimbreMenu::id_pointer);
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
	}


}

// 显示音色菜单界面
void TimbreMenu::showTimbre() {
	TimbreMenu::page = Midi::getTimbre() / 20;
	TimbreMenu::id_pointer = Midi::getTimbre() % 20;
	system("cls");
	std::cout << "MIDI电子琴设置:" << std::endl;
	std::cout << "键入id以选择你的音色 键入q表退出当前界面 按←向前翻页,按→向后翻页 按↑和↓向上向下选中音色 按Enter选定音色" << std::endl;
	std::cout << "音色表(" << TimbreMenu::page + 1 << "/6)" << std::endl;
	TimbreMenu::showPage();
}

// 显示音色表
void TimbreMenu::showPage() {
	for (int i = TimbreMenu::page * 20; i < TimbreMenu::page * 20 + 20 and i < Midi::getInstrumentList()->size(); i++) {
		if (i == TimbreMenu::id_pointer) {
			std::cout << "->  ";
		}
		std::cout << Midi::getInstrumentList()->at(i).id << "     " << Midi::getInstrumentList()->at(i).Chinese_name << std::endl;
	}
}