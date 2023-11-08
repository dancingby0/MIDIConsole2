#include "ScoreMenu.h"




// ָ����ǰ��ɫ���ҳ��
int ScoreMenu::page;  // 10��1ҳ
int ScoreMenu::id_pointer = 0;
int ScoreMenu::file_count = 0;
std::string ScoreMenu::file_name = "";
int ScoreMenu::timbre;
int ScoreMenu::time;  // Ŀǰʱ��
int ScoreMenu::score_time;  // ������ʱ��

std::vector<std::string> ScoreMenu::ScoreList;


bool ScoreMenu::findVectorKey(std::vector<std::string>List, std::string value) {
	bool flag = false;
	for (auto i = List.begin(); i < List.end(); i++) {
		if (*i == value) flag = true;
	}
	return flag;
}


void ScoreMenu::ReadScoreFile() {
	std::string folderPath = "scores";  // ����Ϊ��Ҫ�г��ļ����ļ���·��

	if (std::filesystem::is_directory(folderPath)) {
		for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
			if (std::filesystem::is_regular_file(entry) and entry.path().extension() == ".scores" 
				and not ScoreMenu::findVectorKey(ScoreMenu::ScoreList, entry.path().filename().string())) {
				ScoreMenu::ScoreList.push_back(entry.path().filename().string());  // �����ļ���
				file_count++;
			}
		}
	}
	else {
		std::cerr << "�ļ��� " << folderPath << " �����ڡ�" << std::endl;
		Sleep(1000);
		ScoreMenu::quitScoreMenu();
		return;
	}
	if (file_count == 0) {
		std::cout << "�ļ����ļ�Ϊ��" << std::endl;
		ScoreMenu::quitScoreMenu();
		return;
	}
	else {
		ScoreMenu::showScore();
	}
}

// ��ɫ�˵�����ʽ
void ScoreMenu::runScore() {

	

	switch (Menu::getKey()) {
		// �˳�
	case 'Q':
		ScoreMenu::quitScoreMenu();
		break;
		// ����ҳ��ɫ��
	case VK_LEFT:
		if (ScoreMenu::page > 0) {
			ScoreMenu::page--;
			ScoreMenu::id_pointer = ScoreMenu::page * 10;
			ScoreMenu::showScore();
			Sleep(100);
		};
		break;
		// ���ҷ�ҳ��ɫ��
	case VK_RIGHT:
		if (ScoreMenu::page < (ScoreMenu::file_count-1)/10) {
			ScoreMenu::page++;
			ScoreMenu::id_pointer = ScoreMenu::page * 10;
			ScoreMenu::showScore();
			Sleep(100);
		};
		break;
		// ����ѡ����ɫ
	case VK_UP:
		if (ScoreMenu::id_pointer > ScoreMenu::page * 10) {
			ScoreMenu::id_pointer--;
			ScoreMenu::showScore();
			Sleep(50);
		}
		break;
		// ����ѡ����ɫ
	case VK_DOWN:
		if (ScoreMenu::id_pointer < ScoreMenu::page * 10 + 9 and ScoreMenu::id_pointer < ScoreMenu::file_count - 1) {
			ScoreMenu::id_pointer++;
			ScoreMenu::showScore();
			Sleep(50);
		}
		break;
		// ��������
	case VK_RETURN:

		Menu::setState(SETTING);
		Menu::showSetting();
		break;
	}
}

// ��ʾ��ɫ�˵�����
void ScoreMenu::showScore() {

	system("cls");
	std::cout << "�ļ��б�:" << std::endl;
	std::cout << "����id��ѡ������ļ� ����q���˳���ǰ���� ������ǰ��ҳ,�������ҳ �����͡��������� ��Enter������Ƶ�ļ�" << std::endl;
	std::cout << "�ļ���(" << ScoreMenu::page + 1 << "/" << (ScoreMenu::file_count-1)/10 + 1<<")" << std::endl;
	ScoreMenu::showPage();
}

// ��ʾ��ɫ��
void ScoreMenu::showPage() {
	for (int i = ScoreMenu::page * 10; i < (ScoreMenu::page + 1) * 10 and i < ScoreMenu::file_count; i++) {
		if (i == ScoreMenu::id_pointer) {
			std::cout << "->  ";
		}
		std::cout << ScoreMenu::ScoreList.at(i)<< std::endl;
	}
}

// �뿪�˵�
void ScoreMenu::quitScoreMenu() {

	Menu::setState(MENU);
	Menu::showMenu();
}