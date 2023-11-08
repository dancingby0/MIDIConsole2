#include "Midi.h"



const int Midi::MAJOR[7] = { 2,2,1,2,2,2,1 };
const int Midi::MINOR[7] = { 2,1,2,2,1,2,2 };

const std::array<int, 14> Midi::MODE = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13 };

bool Midi::flag;

int Midi::heart_beat=0;

// 键表
std::map<int, int> Midi::KeyMap;

// MIDI句柄
HMIDIOUT Midi::handle;

// 刚发声的音符表
std::vector<int> Midi::RunSoundList;

// 刚停声的音符表
std::vector<int> Midi::StopSoundList;

// 音色表
std::vector<InstrumentData> Midi::InstrumentList;

// 正在播放的音调表
std::vector<int> Midi::SoundList;

// 存储1tick时玩家按下的键的id
std::vector<int> Midi::KeyList;

// 存储玩家实时按下的键的id
std::vector<int> Midi::RealTimeKeyList;

// 判定vector中是否存在一个值

bool Midi::findVectorKey(std::vector<int>List, int value) {
	bool flag = false;
	for (auto i = List.begin(); i < List.end(); i++) {
		if (*i == value) flag = true;
	}
	return flag;
}

// UTF-8转GB
std::string UTF8ToGB(const char* str)
{
	std::string result;
	WCHAR* strSrc;
	LPSTR szRes;

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//获得临时变量的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}

 HMIDIOUT Midi::getHandle() {
	 return Midi::handle;
}

// 获取Midi的调式
int Midi::getMode() {
	return MidiConfig::getMode();
}

// 设定Midi的调式
void Midi::setMode(int mode) {
	MidiConfig::setMode(mode);
}

// 获取调式列表
const std::array<int, 14>* Midi::getModeList() {
	return &Midi::MODE;
}

// 获取音色表
std::vector<InstrumentData>* Midi::getInstrumentList() {
	return &Midi::InstrumentList;
}

// 获取正在按下的键
std::vector<int>* Midi::getKeyList() {
	return &Midi::RealTimeKeyList;
}

// 获取正在播放的声音
std::vector<int>* Midi::getSoundList() {
	return &Midi::SoundList;
}

// 获取刚发声时的音符
std::vector<int>* Midi::getRunSoundList() {
	return &Midi::RunSoundList;
}

// 获取刚停声的音符
std::vector<int>* Midi::getStopSoundList() {
	return &Midi::StopSoundList;
}

// 获取MIDI电子琴的心跳
bool Midi::getHeartBeat() {
	return Midi::heart_beat;
}

// 获取MIDI电子琴当前的音色
int Midi::getTimbre() {
	return MidiConfig::getTimbre();
}

// 获取MIDI电子琴当前的响度
int Midi::getVolume() {
	return MidiConfig::getVolume();
}


// 设定音色
void Midi::setTimbre(int id) {
	MidiConfig::setTimbre(id);
}

// 设定响度
void Midi::setVolume(int volume) {
	MidiConfig::setVolume(volume);
}

// 获取时间刻
int Midi::getTick() {
	return MidiConfig::getTick();
}

// 设定时间刻
void Midi::setTick(int tick) {
	MidiConfig::setTick(tick);
}

// 生成键与音调的对应表
void Midi::summonKeyMap() {
	// 设定按下可发声的键
	std::string keys = "QWERTYUASDFGHJZXCVBNM";

	// 大调式
	if (MidiConfig::getMode() <= 6 and MidiConfig::getMode() >= 0) {
		Midi::KeyMap['Q'] = 45 + MidiConfig::getMode();// 设定第一个音
		for (int i = 1; i < keys.size(); i++) {
			Midi::KeyMap[keys[i]] = Midi::KeyMap[keys[i - 1]] + Midi::MAJOR[(i - 1) % 7];
		}
	}
	// 小调式
	else {
		Midi::KeyMap['Q'] = 45 + MidiConfig::getMode() - 7;// 设定第一个音,45为A的ID
		for (int i = 1; i < keys.size(); i++) {
			Midi::KeyMap[keys[i]] = Midi::KeyMap[keys[i - 1]] + Midi::MINOR[(i - 1) % 7];
		}
	}
}

// 从timbre.txt载入音色
void Midi::initialTimbre() {



	std::ifstream inputFile("timbre.txt");
	// 开启文件
	if (!inputFile.is_open()) {
		std::cerr << "错误!无法打开文件以读取音色!" << std::endl;
	}
	else {
		std::string line;
		// 对每一行进行操作
		while (std::getline(inputFile, line)) {
			line = UTF8ToGB(line.c_str()).c_str();// 文件中UTF-8转为GB
			int id = 0;
			std::string name;
			std::string Chinese_name;
			int state = 0;// 读取一行时初始状态(读取id)

			for (int i = 0; i < line.size(); i++) {

				// 若id在数字范围内并且时状态0时,读取
				if ('0' <= line[i] and line[i] <= '9' and state == 0) {
					id = id * 10 + (line[i] - '0');
				}
				// 若不是数字(空格),且状态位于0(读取数字时),则转换为1(读取name)
				else if (state == 0) {
					state = 1;
				}
				// 若状态为name,且为 字母 或 空格 或 大括号 或 小括号 或 数字 时读为name
				if (state == 1 // 状态1
					and ('A' <= line[i] and 'Z' >= line[i] or 'a' <= line[i] and 'z' >= line[i]  // 字母
						or line[i] == ' ' or line[i] == '(' or line[i] == ')' or line[i] == '+' or line[i] == '-'  // 其他符号
						or line[i] <= '9' and '0' <= line[i])) {
					name += line[i];
				}
				// 否则(若状态为1),读的是其他字符(汉字),状态为2(读取汉字)
				else if (state == 1) {
					state = 2;
				}
				// 若状态为2,则读取中文
				if (state == 2) {
					Chinese_name += line[i];
				}
			}
			if (state == 2) {
				Midi::InstrumentList.push_back({ id, name, Chinese_name });
			}
		}
		inputFile.close();
	}
}

// 初始化MIDI电子琴程式
void Midi::initialMidi() {
	midiOutOpen(&Midi::handle, 0, 0, 0, CALLBACK_NULL); // 开启midi设备
	MidiConfig::readConfigFile(); // 读取配置文件
	Midi::initialTimbre();
	// 初始化MIDI电子琴数据
	Midi::flag = false;
}

// 运行电子琴
void Midi::runMIDI() {
	Midi::flag = true;
	Midi::showCliPrompts();  // 显示提示语
	midiOutShortMsg(Midi::handle, MidiConfig::getTimbre() << 8 | 0xC0);  // 设定音色
	Midi::summonKeyMap();// 生成键与音符对应表
	Midi::heart_beat = 0;


	// 外层循环,1ms循环一次,能够时时刻刻获取键
	// 内层发声循环,遵循tick节拍,能够规范声音的发送.
	while (Midi::flag) {

		Midi::detectKeyboardInput();  // 检测键的实时输入

		Midi::operateRealTimeKey();  // 对键操作(实时操作)

		// 此处放置外接功能
		Recording::runRecording();  // 运行录制系统


		Midi::disposeRest();
		heart_beat++;
		Sleep(1);  // 外层循环1ms一次
	}
}

// 检测玩家实时输入的键
void Midi::detectKeyboardInput() {
	for (int key = 1; key < 256; key++) {
		// 探测键是否按下(第16位为按下的判断位)
		if (GetKeyState(key) & 0x8000) {
			// 塞入实时监测的表
			Midi::RealTimeKeyList.push_back(key);
		}
	}

	for (int i : Midi::RealTimeKeyList) {
		// 若1tick的键表中找不到实时检测的表中玩家的输入,塞进去
		if (not findVectorKey(KeyList, i)) {
			Midi::KeyList.push_back(i);
		}
	}
}

// 处理杂项
// 当heart_beat与tick相等时,处理1tick所有的事件
void Midi::disposeRest() {
	Midi::operateKey();  //执行一次tick时键的相应操作 发声,停声
	Midi::RealTimeKeyList.clear(); // 当读取下一次实时数据时,清空玩家键表
	

	// 1tick结束,处理1tick发生的事件
	if (Midi::heart_beat >= MidiConfig::getTick()) {
		

		// 这里插入1tick时要进行的其他功能(获取发声表和停声表)
		Recording::runRecordingTick();

		Midi::heart_beat = 0;  // 开始新的一次心跳
		Midi::KeyList.clear(); // 当读取下1tick数据时,清空玩家键表
		// 清空发声表,停声表
		Midi::RunSoundList.clear();
		Midi::StopSoundList.clear();
	}
}

// 检测键实时的输入
void Midi::detectRealTimeKeyList() {
	
	for (int i : Midi::RealTimeKeyList) {
		// 若1tick的键表中找不到实时检测的表中玩家的输入,塞进去
		if (not findVectorKey(KeyList, i)) {
			Midi::KeyList.push_back(i);
		}
	}
}

// 根据玩家实时按下的键执行操作
// 执行 : 退出
void Midi::operateRealTimeKey() {
	for (int i : Midi::RealTimeKeyList) {
		
		switch (i) {

			// 按下数字9,则退出
		case '9':
			Midi::quit();
			break;

			// 按下小键盘9,退出
		case VK_NUMPAD9:
			Midi::quit();
			break;
		}
	}
}

// 对玩家1tick输入的键进行相应操作
// 执行 : 发声,停声
void Midi::operateKey() {
	for (int i : Midi::RealTimeKeyList) {
		// 当键表里的键不在Sound里,说明第一次按下,发声
		if (not findVectorKey(Midi::SoundList, i) and Midi::KeyMap.count(i) > 0) {
			Midi::playSound(Midi::handle, Midi::KeyMap[i], MidiConfig::getVolume());
			
			if (not findVectorKey(Midi::RunSoundList, i)){
				Midi::RunSoundList.push_back(KeyMap[i]);  // 将1tick内发声的音调存入RunSoundList中
			}
			Midi::SoundList.push_back(i);
		}
	}
	// 当Sound表里的音调不在键表里,说明松开了,停声
	for (int i : Midi::SoundList) {
		if (not findVectorKey(Midi::RealTimeKeyList, i)) {
			Midi::playSound(Midi::handle, Midi::KeyMap[i], 0);
			if (not findVectorKey(Midi::StopSoundList, KeyMap[i])) {
				Midi::StopSoundList.push_back(KeyMap[i]);  // 将1tick内停声的音调存入StopSoundList中
			}
			[](std::vector<int>& List, int key) {List.erase(std::remove(List.begin(), List.end(), key), List.end()); }(Midi::SoundList, i);
		}
	}
}

// 显示提示语(CLI)
void Midi::showCliPrompts() {
	// 判定是否在命令行界面下
	if (_isatty(_fileno(stdout))) {
		system("cls");
		// 对调式进行文本推理
		std::string tmp;
		if (MidiConfig::getMode() < 7) {
			tmp = MidiConfig::getMode() + 'A';
			tmp += "大调式";
		}
		else {
			tmp = MidiConfig::getMode() - 7 + 'A';
			tmp += "小调式";
		}
		std::cout << "你目前的调式为: " << tmp << "   音色为:" << Midi::InstrumentList[MidiConfig::getTimbre()].Chinese_name << "  响度为 : " << MidiConfig::getVolume() << std::endl;
		std::cout << "输入数字9退出" << std::endl;
		std::cout << "A ~ K 分别对应 do3 ~ do4" << std::endl;
		// 启动了录制
		if (Recording::getState()) {
			std::cout << "按下O暂停录制,按下P暂停录制" << std::endl;
		}
		else {
			std::cout << "按下P进行录制弹奏的乐曲" << std::endl;
		}
		std::cout << "请输入你的按键:" << std::endl;
		// 正在录制时
		switch (Recording::getState()) {
		case Recording::STATE::PAUSE:
			std::cout << "暂停录制" << std::endl;
			break;
		case Recording::STATE::RECORDING:
			std::cout << "正在录制中" << std::endl;
			break;
		}
	}
}

// 播放声音
void Midi::playSound(HMIDIOUT handle, int frequency, int volume) {
	midiOutShortMsg(handle, frequency << 8 | 0x90 | volume << 16);

}

// 暂停声音
void Midi::stopSound(HMIDIOUT handle, int frequency) {
	midiOutShortMsg(handle, frequency << 8 | 0x90 | 0 << 16);
}

// 保存配置信息
void Midi::writeConfigFile() {
	MidiConfig::writeConfigFile();
}

// 退出电子琴弹奏
void Midi::quit() {
	Midi::KeyList.clear(); // 清空玩家的输入
	Menu::setState(MENU); // 设置菜单的状态为"主菜单"
	Menu::showMenu();
	Midi::flag = false; // 退出主程式
}
