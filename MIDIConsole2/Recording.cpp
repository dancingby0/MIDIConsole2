#include "Recording.h"

const double Recording::TICK = 0.03125;
int Recording::total_tick = 0;


std::string Recording::file_name = "";

int Recording::state = DONE;


// 获取录制状态
int Recording::getState() {
	return Recording::state;
}



// 运行录制系统
void Recording::runRecording() {
	for (int i : *Midi::getKeyList()) {

		if (i == 'P') {
			// 当正在录制时
			switch (Recording::getState()) {
				// 启动录制
			case Recording::STATE::DONE:
				Recording::startRecording();
				break;

				// 停止录制
			case Recording::STATE::PAUSE:
				Recording::stopRecording();
				break;

				//停止录制
			case Recording::STATE::RECORDING:
				Recording::stopRecording();
				break;
			}
			// 更新面板
			Midi::showCliPrompts();
		}
		else if (i == 'O' and Recording::state == RECORDING) {
			// 切换暂停状态
			Recording::switchPause();
		}

	}


}
// 开始录制
void Recording::startRecording() {
	std::string input;
	system("cls");
	Sleep(200);  // 暂停200ms,以防止误触
	std::string folder = "./scores/";

	
	// 若文件夹不存在,创建
	if (_access(folder.c_str(), 0))
	{
		//system("mkdir head");
		int success = _mkdir(folder.c_str());
	}

	bool flag_input = true;
	while (flag_input) {
		std::cout << "请输入保存的录制音频文件名(输入\":wq\"取消,仅可输入字母与数字,若目录中存在相同的文件则会被覆盖):" << std::endl;
		std::cin >> input;
		bool flag = true;
		for (char i : input) {
			if (not(i <= 9 and i >= 0 or i <= 'z' and i >= 'a' or i <= 'Z' and i >= 'A')) {
				flag = false;
			}
		}
		if (input == ":wq") {
			return;  // 退出录制
		}
		// 文件命名规范
		else if (flag) {
			flag_input = false;
			Recording::file_name = input;

		}
		// 文件命名不规范
		else {
			std::cout << "你输入了非法字符!请重新输入!" << std::endl;
			Sleep(1000);
			system("cls");
		}
	}
	Recording::state = 1;
}


void Recording::switchPause() {
	if (Recording::state == STATE::RECORDING) {
		Recording::state = STATE::PAUSE;
	}
	else {
		Recording::state = STATE::RECORDING;
	}
	
}


void Recording::stopRecording() {
	Recording::state = STATE::DONE;
}