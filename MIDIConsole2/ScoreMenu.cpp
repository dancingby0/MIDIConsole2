#include "ScoreMenu.h"




// ָ����ǰ��ɫ���ҳ��
int ScoreMenu::page;  // 10��1ҳ
int ScoreMenu::id_pointer = 0;
int ScoreMenu::file_count = 0;
std::string ScoreMenu::file_name = "";
int ScoreMenu::timbre;
int ScoreMenu::time;  // Ŀǰʱ��
int ScoreMenu::score_time;  // ������ʱ��

std::vector<ScoreMenuSound> ScoreMenu::SoundList;
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

		ScoreMenu::quitScoreMenu();
		return;
	}
	if (file_count == 0) {
		system("cls");
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

		ScoreMenu::playScore();

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
	Sleep(1000);
	system("cls");
	Menu::setState(MENU);
	Menu::showMenu();
}

void ScoreMenu::playScore() {
	ScoreMenu::score_time = -1;
	ScoreMenu::file_name = ScoreMenu::ScoreList.at(ScoreMenu::id_pointer);

	// �����ļ�������
	std::ifstream inputFile("scores/" + file_name);
	ScoreMenu::time = 0;
	if (inputFile.is_open()) {
		std::string line;

		// ���ж�ȡ�ļ�����
		while (std::getline(inputFile, line)) {
			// �����ﴦ��ÿһ�е�����
			disposeLine(line);
			
		}

		// �ر��ļ�
		inputFile.close();
	}
	else {
		std::cerr << "�޷����ļ�: " << file_name << std::endl;
		ScoreMenu::quitScoreMenu();
		return ;
	}

	bool flag = true;
	while (flag) {
		Sleep(1);
		ScoreMenu::time++;
		for (ScoreMenuSound i : ScoreMenu::SoundList) {
			if (i.time <= ScoreMenu::time and i.type == 0) {
				Midi::stopSound(Midi::getHandle(), i.frequency);
				[](std::vector<ScoreMenuSound>& List, ScoreMenuSound key) {
					List.erase(std::remove(List.begin(), List.end(), key), List.end());
					}(ScoreMenu::SoundList, i);
			}
			else if (i.time <= ScoreMenu::time and i.type == 1) {
				Midi::playSound(Midi::getHandle(), i.frequency,i.volume);
				[](std::vector<ScoreMenuSound>& List, ScoreMenuSound key) {
					List.erase(std::remove(List.begin(), List.end(), key), List.end());
					}(ScoreMenu::SoundList, i);
			}
		}
		if (ScoreMenu::time > ScoreMenu::score_time) {
			flag = false;
			ScoreMenu::quitScoreMenu();
		}
	}

}

void ScoreMenu::disposeLine(std::string line) {
	// �ָ�ÿһ�еļ�ֵ��
	std::istringstream iss(line);
	std::string key, value;
	if (std::getline(iss, key, ':') && std::getline(iss, value)) {
		if (key == "timbre") {
			ScoreMenu::timbre = std::stoi(value);
		}
		else if (key == "total_time") {
			ScoreMenu::score_time = std::stoi(value);
		}
	}
	// ���޷��ָ�,��Ϊ������
	else {
		int state = 0;
		std::string num[] = { "" ,"","",""};
		for (char i = 0; i < line.size(); i++){
			if (line[i] <= '9' and line[i] >= '0') {
				num[state] += line[i];
			}
			//����ǿո�
			else if(line[i]==' ') {
				state += 1;
			}
		}
		ScoreMenu::SoundList.push_back(ScoreMenuSound{ std::stoi(num[0]),std::stoi(num[1]),std::stoi(num[2]) ,std::stoi(num[3])});
	}
}