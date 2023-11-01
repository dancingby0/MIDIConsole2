#include "TimbreMenu.h"



// ָ����ǰ��ɫ���ҳ��
int TimbreMenu::page;
int TimbreMenu::id_pointer;

// ��ɫ�˵�����ʽ
void TimbreMenu::runTimbre() {
	
	switch (Menu::getKey()){
	// �˳�
	case 'Q':
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
	// ����ҳ��ɫ��
	case VK_LEFT:
		if (TimbreMenu::page > 0) {
			TimbreMenu::page--;
			TimbreMenu::id_pointer = TimbreMenu::page * 20;
			TimbreMenu::showTimbre();
			Sleep(100);
		};
		break;
	// ���ҷ�ҳ��ɫ��
	case VK_RIGHT:
		if (TimbreMenu::page < 5) {
			TimbreMenu::page++;
			TimbreMenu::id_pointer = TimbreMenu::page * 20;
			TimbreMenu::showTimbre();
			Sleep(100);
		};
		break;
	// ����ѡ����ɫ
	case VK_UP:
		if (TimbreMenu::id_pointer > TimbreMenu::page * 20) {
			TimbreMenu::id_pointer--;
			TimbreMenu::showTimbre();
			Sleep(50);
		}
		break;
	// ����ѡ����ɫ
	case VK_DOWN:
		if (TimbreMenu::id_pointer < TimbreMenu::page * 20 + 19 and TimbreMenu::id_pointer < Midi::getInstrumentList()->size() - 1) {
			TimbreMenu::id_pointer++;
			TimbreMenu::showTimbre();
			Sleep(50);
		}
		break;
	// ��������
	case VK_RETURN:
		Midi::setTimbre(TimbreMenu::id_pointer);
		Menu::setState(SETTING);
		Menu::showSetting();
		break;
	}


}

// ��ʾ��ɫ�˵�����
void TimbreMenu::showTimbre() {
	TimbreMenu::page = Midi::getTimbre() / 20;
	TimbreMenu::id_pointer = Midi::getTimbre() % 20;
	system("cls");
	std::cout << "MIDI����������:" << std::endl;
	std::cout << "����id��ѡ�������ɫ ����q���˳���ǰ���� ������ǰ��ҳ,�������ҳ �����͡���������ѡ����ɫ ��Enterѡ����ɫ" << std::endl;
	std::cout << "��ɫ��(" << TimbreMenu::page + 1 << "/6)" << std::endl;
	TimbreMenu::showPage();
}

// ��ʾ��ɫ��
void TimbreMenu::showPage() {
	for (int i = TimbreMenu::page * 20; i < TimbreMenu::page * 20 + 20 and i < Midi::getInstrumentList()->size(); i++) {
		if (i == TimbreMenu::id_pointer) {
			std::cout << "->  ";
		}
		std::cout << Midi::getInstrumentList()->at(i).id << "     " << Midi::getInstrumentList()->at(i).Chinese_name << std::endl;
	}
}