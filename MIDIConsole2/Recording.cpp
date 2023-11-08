#include "Recording.h"


int Recording::total_time = 0;
bool Recording::heart_beat = true;

std::string Recording::file_name = "";

int Recording::state = DONE;



// 获取录制状态
int Recording::getState() {
	return Recording::state;
}

// 运行录制系统.每tick运行一次
void Recording::runRecordingTick() {

	// 正在录制的状态且心跳相同时
	if (Recording::getState() == Recording::STATE::RECORDING) {
		total_time += Midi::getTick();  // 增加总时长(此处单位为ms)
		Recording::writeFile();
	}
}
// 运行录制系统,每1ms运行一次
void Recording::runRecording() {


	for (int i : *Midi::getKeyList()) {

		// 按下录制键时
		if (i == 'P') {
			Sleep(200);  // 暂停200ms,以防止误触
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

		// 按下切换暂停键时
		else if (i == 'O' and not(Recording::getState() == Recording::STATE::DONE)) {
			Recording::switchPause();
			Sleep(200);
		}
	}
}


// 开始录制
void Recording::startRecording() {

	system("cls");
	Recording::newFile();

}


void Recording::switchPause() {

	if (Recording::state == STATE::RECORDING) {
		Recording::state = STATE::PAUSE;


	}
	else {
		Recording::state = STATE::RECORDING;

	}
	Midi::showCliPrompts();

}


void Recording::stopRecording() {
	// 打开录制文件
	std::ofstream outputFile("scores/" + Recording::file_name, std::ios::app);

	if (outputFile.is_open()) {
		// 向文件中写入要停止的键
		outputFile << "total_time: " << total_time << std::endl;

		outputFile.close();
	}
	else {
		std::cerr << "无法打开文件 " << Recording::file_name << "。" << std::endl;
		Sleep(1000);
	}

	Recording::total_time = 0;
	Recording::file_name = "";
	Recording::state = STATE::DONE;
}



// 创建新的录制文件
void Recording::newFile() {

	std::string folder = "scores";

	// 检查文件夹是否存在，如果不存在则创建它
	if (not std::filesystem::is_directory(folder)) {
		std::filesystem::create_directory(folder);
	}

	bool flag_input = true;
	while (flag_input) {
		std::string input;
		
		
		system("cls");
		std::cout << "请输入保存的录制音频文件名(输入\"QUIT\"取消,仅可输入字母与数字,若目录中存在相同的文件则会被覆盖):" << std::endl;
		bool flag_input_0 = true;
		while (flag_input_0) {
			
			for (char key = 1; key < 256; key++) {
				// 探测键是否被按下
				if (GetKeyState(key) & 0x8000 and (key <= 'z' and key >= 'a' or key <= 'Z' and key >= 'A' or key <= '9' and key >= '0')) {
					std::cout << key;
					input += key;
					Sleep(200);
				}
				if (GetKeyState(key) & 0x8000 and key == VK_RETURN) {
					flag_input_0 = false;
					break;
				}
				if (GetKeyState(key) & 0x8000 and key == VK_BACK and not input.empty()) {
					system("cls");
					std::cout << "请输入保存的录制音频文件名(输入\":wq\"取消,仅可输入字母与数字,若目录中存在相同的文件则会被覆盖):" << std::endl;
					input.pop_back();
					std::cout << input;
					Sleep(200);
				}
				
			}
		}
		bool flag = true;
		for (char i : input) {
			if (not(i <= 9 and i >= 0 or i <= 'z' and i >= 'a' or i <= 'Z' and i >= 'A')) {
				flag = false;
			}
		}
		if (input == "QUIT") {

			return;  // 退出录制
		}

		// 文件命名规范
		else if (flag) {
			Recording::file_name = input + ".scores";

			// 检查文件是否存在
			if (std::filesystem::exists(folder + "/" + Recording::file_name)) {
				system("cls");
				std::cout << "文件 " << Recording::file_name << " 已存在!" << std::endl;
				Recording::file_name = "";
				Sleep(1000);
			}
			else {
				// 打开文件，如果文件不存在则创建它,有则清空
				std::ofstream outputFile("scores/" + Recording::file_name, std::ios::ate);

				if (outputFile.is_open()) {
					// 向文件中写入初始内容
					outputFile << "timbre:" << Midi::getTimbre() << std::endl;
					outputFile << "volume:" << Midi::getVolume() << std::endl;
					outputFile << "mode:" << Midi::getMode() << std::endl;
					outputFile.close();
				}
				else {
					std::cerr << "无法打开文件 " << Recording::file_name << "。" << std::endl;
					Sleep(1000);
				}
				flag_input = false;
				Recording::state = Recording::STATE::RECORDING;
				Sleep(100);
			}
		}
		// 文件命名不规范
		else {
			system("cls");
			std::cout << "你输入了非法字符!请重新输入!" << std::endl;
			Sleep(1000);

		}
	}
}



void Recording::writeFile() {

	

	// 读取发声的列表
	for (int i : *Midi::getRunSoundList()) {
		// 若Run和Stop表中都存在同一个键,则不存储
		if (Midi::findVectorKey(*Midi::getStopSoundList(), i)) {
			continue;
		}
		// 打开录制文件
		std::ofstream outputFile("scores/" + Recording::file_name, std::ios::app);

		if (outputFile.is_open()) {
			// 向文件中写入要发声的键
			outputFile << "1 " << total_time << " " << i << std::endl;

			outputFile.close();
		}
		else {
			std::cerr << "无法打开文件 " << Recording::file_name << "。" << std::endl;
			Sleep(1000);
		}
	}

	// 读取停声的列表
	for (int i : *Midi::getStopSoundList()) {
		if (Midi::findVectorKey(*Midi::getRunSoundList(), i)) {
			continue;
		}
		// 打开录制文件
		std::ofstream outputFile("scores/" + Recording::file_name, std::ios::app);

		if (outputFile.is_open()) {
			// 向文件中写入要停止的键
			outputFile << "0 " << total_time << " " << i << std::endl;

			outputFile.close();
		}
		else {
			std::cerr << "无法打开文件 " << Recording::file_name << "。" << std::endl;
			Sleep(1000);
		}
	}

}