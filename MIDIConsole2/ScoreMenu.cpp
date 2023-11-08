#include "ScoreMenu.h"




// 指明当前音色表的页数
int ScoreMenu::page;  // 10个1页
int ScoreMenu::id_pointer = 0;
int ScoreMenu::file_count = 0;
std::string ScoreMenu::file_name = "";
int ScoreMenu::timbre;
int ScoreMenu::time;  // 目前时长
int ScoreMenu::score_time;  // 谱子总时长

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
	std::string folderPath = "scores";  // 更改为你要列出文件的文件夹路径

	if (std::filesystem::is_directory(folderPath)) {
		for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
			if (std::filesystem::is_regular_file(entry) and entry.path().extension() == ".scores" 
				and not ScoreMenu::findVectorKey(ScoreMenu::ScoreList, entry.path().filename().string())) {
				ScoreMenu::ScoreList.push_back(entry.path().filename().string());  // 塞进文件名
				file_count++;
			}
		}
	}
	else {
		std::cerr << "文件夹 " << folderPath << " 不存在。" << std::endl;

		ScoreMenu::quitScoreMenu();
		return;
	}
	if (file_count == 0) {
		system("cls");
		std::cout << "文件夹文件为空" << std::endl;

		ScoreMenu::quitScoreMenu();
		return;
	}
	else {
		ScoreMenu::showScore();
	}
}

// 音色菜单主程式
void ScoreMenu::runScore() {

	

	switch (Menu::getKey()) {
		// 退出
	case 'Q':
		ScoreMenu::quitScoreMenu();
		break;
		// 向左翻页音色表
	case VK_LEFT:
		if (ScoreMenu::page > 0) {
			ScoreMenu::page--;
			ScoreMenu::id_pointer = ScoreMenu::page * 10;
			ScoreMenu::showScore();
			Sleep(100);
		};
		break;
		// 向右翻页音色表
	case VK_RIGHT:
		if (ScoreMenu::page < (ScoreMenu::file_count-1)/10) {
			ScoreMenu::page++;
			ScoreMenu::id_pointer = ScoreMenu::page * 10;
			ScoreMenu::showScore();
			Sleep(100);
		};
		break;
		// 向上选择音色
	case VK_UP:
		if (ScoreMenu::id_pointer > ScoreMenu::page * 10) {
			ScoreMenu::id_pointer--;
			ScoreMenu::showScore();
			Sleep(50);
		}
		break;
		// 向下选择音色
	case VK_DOWN:
		if (ScoreMenu::id_pointer < ScoreMenu::page * 10 + 9 and ScoreMenu::id_pointer < ScoreMenu::file_count - 1) {
			ScoreMenu::id_pointer++;
			ScoreMenu::showScore();
			Sleep(50);
		}
		break;
		// 保存设置
	case VK_RETURN:

		ScoreMenu::playScore();

		break;
	}
}

// 显示音色菜单界面
void ScoreMenu::showScore() {

	system("cls");
	std::cout << "文件列表:" << std::endl;
	std::cout << "键入id以选择你的文件 键入q表退出当前界面 按←向前翻页,按→向后翻页 按↑和↓向上向下 按Enter播放音频文件" << std::endl;
	std::cout << "文件表(" << ScoreMenu::page + 1 << "/" << (ScoreMenu::file_count-1)/10 + 1<<")" << std::endl;
	ScoreMenu::showPage();
}

// 显示音色表
void ScoreMenu::showPage() {
	for (int i = ScoreMenu::page * 10; i < (ScoreMenu::page + 1) * 10 and i < ScoreMenu::file_count; i++) {
		if (i == ScoreMenu::id_pointer) {
			std::cout << "->  ";
		}
		std::cout << ScoreMenu::ScoreList.at(i)<< std::endl;
	}
}

// 离开菜单
void ScoreMenu::quitScoreMenu() {
	Sleep(1000);
	system("cls");
	Menu::setState(MENU);
	Menu::showMenu();
}

void ScoreMenu::playScore() {
	ScoreMenu::score_time = -1;
	ScoreMenu::file_name = ScoreMenu::ScoreList.at(ScoreMenu::id_pointer);

	// 创建文件输入流
	std::ifstream inputFile("scores/" + file_name);
	ScoreMenu::time = 0;
	if (inputFile.is_open()) {
		std::string line;

		// 逐行读取文件内容
		while (std::getline(inputFile, line)) {
			// 在这里处理每一行的内容
			disposeLine(line);
			
		}

		// 关闭文件
		inputFile.close();
	}
	else {
		std::cerr << "无法打开文件: " << file_name << std::endl;
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
	// 分割每一行的键值对
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
	// 若无法分割,则为发声键
	else {
		int state = 0;
		std::string num[] = { "" ,"","",""};
		for (char i = 0; i < line.size(); i++){
			if (line[i] <= '9' and line[i] >= '0') {
				num[state] += line[i];
			}
			//如果是空格
			else if(line[i]==' ') {
				state += 1;
			}
		}
		ScoreMenu::SoundList.push_back(ScoreMenuSound{ std::stoi(num[0]),std::stoi(num[1]),std::stoi(num[2]) ,std::stoi(num[3])});
	}
}