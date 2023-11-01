#include "Midi.h"

const int Midi::MAJOR[7] = { 2,2,1,2,2,2,1 };
const int Midi::MINOR[7] = { 2,1,2,2,1,2,2 };

const std::array<int, 14> Midi::MODE = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13 };

bool Midi::flag;

// ����
std::map<int, int> Midi::KeyMap;

// MIDI���
HMIDIOUT Midi::handle;

// ��ɫ��
std::vector<InstrumentData> Midi::InstrumentList;

// ���ڲ��ŵ�������
std::vector<int> Midi::SoundList;

// �洢1tickʱ��Ұ��µļ���id
std::vector<int> Midi::KeyList;

// �ж�vector���Ƿ����һ��ֵ
bool inline findVectorKey(std::vector<int>List, int value) {
	bool flag = false;
	for (auto i = List.begin(); i < List.end(); i++) {
		if (*i == value) flag = true;
	}
	return flag;
}

// UTF-8תGB
std::string UTF8ToGB(const char* str)
{
	std::string result;
	WCHAR* strSrc;
	LPSTR szRes;

	//�����ʱ�����Ĵ�С
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//�����ʱ�����Ĵ�С
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}

// ��ȡMidi�ĵ�ʽ
int Midi::getMode() {
	return MidiConfig::getMode();
}

// �趨Midi�ĵ�ʽ
void Midi::setMode(int mode) {
	MidiConfig::setMode(mode);
}

// ��ȡ��ʽ�б�
const std::array<int, 14>* Midi::getModeList() {
	return &Midi::MODE;
}

// ��ȡ��ɫ��
std::vector<InstrumentData>* Midi::getInstrumentList() {
	return &Midi::InstrumentList;
}

// ��ȡMIDI�����ٵ�ǰ����ɫ
int Midi::getTimbre() {
	return MidiConfig::getTimbre();
}

// ��ȡMIDI�����ٵ�ǰ�����
int Midi::getVolume() {
	return MidiConfig::getVolume();
}


// �趨��ɫ
void Midi::setTimbre(int id) {
	MidiConfig::setTimbre(id);
}

// �趨���
void Midi::setVolume(int volume) {
	MidiConfig::setVolume(volume);
}

// ���ɼ��������Ķ�Ӧ��
void Midi::summonKeyMap() {
	// �趨���¿ɷ����ļ�
	std::string keys = "QWERTYUASDFGHJZXCVBNM";

	// ���ʽ
	if (MidiConfig::getMode() <= 6 and MidiConfig::getMode() >= 0) {
		Midi::KeyMap['Q'] = 45 + MidiConfig::getMode();// �趨��һ����
		for (int i = 1; i < keys.size(); i++) {
			Midi::KeyMap[keys[i]] = Midi::KeyMap[keys[i - 1]] + Midi::MAJOR[(i - 1) % 7];
		}

	}
	// С��ʽ
	else {
		Midi::KeyMap['Q'] = 45 + MidiConfig::getMode() - 7;// �趨��һ����,45ΪA��ID
		for (int i = 1; i < keys.size(); i++) {
			Midi::KeyMap[keys[i]] = Midi::KeyMap[keys[i - 1]] + Midi::MINOR[(i - 1) % 7];
		}
	}
}

// ��timbre.txt������ɫ
void Midi::initialTimbre() {

	std::ifstream inputFile("timbre.txt");
	// �����ļ�
	if (!inputFile.is_open()) {
		std::cerr << "����!�޷����ļ��Զ�ȡ��ɫ!" << std::endl;
	}
	else {
		std::string line;
		// ��ÿһ�н��в���
		while (std::getline(inputFile, line)) {
			line = UTF8ToGB(line.c_str()).c_str();// �ļ���UTF-8תΪGB
			int id = 0;
			std::string name;
			std::string Chinese_name;
			int state = 0;

			for (int i = 0; i < line.size(); i++) {
				if ('0' <= line[i] and line[i] <= '9' and state == 0) {
					id = id * 10 + (line[i] - '0');
				}
				else {
					state = 1;
				}
				if (state == 1 and ('A' <= line[i] and 'Z' >= line[i] or 'a' <= line[i] and 'z' >= line[i]) or line[i] == ' ' or line[i] == '(' or line[i] == ')' or line[i] <= '9' and '0' <= line[i]) {
					name += line[i];
				}
				else if (state == 1) {
					state = 2;
				}
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

// ��ʼ��MIDI�����ٳ�ʽ
void Midi::initialMidi() {
	midiOutOpen(&Midi::handle, 0, 0, 0, CALLBACK_NULL); // ����midi�豸
	MidiConfig::readConfigFile(); // ��ȡ�����ļ�
	Midi::initialTimbre();
	// ��ʼ��MIDI����������
	Midi::flag = false;
}

// ���е�����
void Midi::runMIDI() {
	Midi::flag = true;
	Midi::showCliPrompts(); // ��ʾ��ʾ��
	midiOutShortMsg(Midi::handle, MidiConfig::getTimbre() << 8 | 0xC0);// �趨��ɫ
	Midi::summonKeyMap();// ���ɼ���������Ӧ��
	while (Midi::flag) {
		Midi::detectKeyboardInput(); // ����������
		Midi::operateKey(); // �Լ�����
		Sleep(10);
	}
}

// ��������
void Midi::detectKeyboardInput() {
	Midi::KeyList.clear(); // ����ȡ��1tick����ʱ,�����Ҽ���
	for (int key = 1; key < 256; key++) {
		// ̽����Ƿ���(��16λΪ���µ��ж�λ)
		if (GetKeyState(key) & 0x8000) {
			Midi::KeyList.push_back(key);
		}
	}
}

// ���������ļ�������Ӧ����
void Midi::operateKey() {


	for (int i : Midi::KeyList) {
		// ��������ļ�����Sound��,˵����һ�ΰ���,����
		if (not findVectorKey(SoundList, i) and Midi::KeyMap.count(i) > 0) {
			Midi::playSound(Midi::handle, Midi::KeyMap[i], MidiConfig::getVolume());
			SoundList.push_back(i);
		}


		// ����1tick����Ұ��µļ�ִ�в���
		switch (i) {

			// ��������9,���˳�
		case '9':
			Midi::quit();
			break;

			// ����С����9,�˳�
		case VK_NUMPAD9:
			Midi::quit();
			break;
		}
	}
	// ��Sound������������ڼ�����,˵���ɿ���,ͣ��
	for (int i : Midi::SoundList) {
		if (not findVectorKey(KeyList, i)) {
			Midi::playSound(Midi::handle, Midi::KeyMap[i], 0);
			[](std::vector<int>& List, int key) {List.erase(std::remove(List.begin(), List.end(), key), List.end()); }(SoundList, i);
		}
	}
}

// ��ʾ��ʾ��
void Midi::showCliPrompts() {
	// �ж��Ƿ��������н�����
	if (_isatty(_fileno(stdout))) {
		system("cls");
		// �Ե�ʽ�����ı�����
		std::string tmp;
		if (MidiConfig::getMode() < 7) {
			tmp = MidiConfig::getMode() + 'A';
			tmp += "���ʽ";
		}
		else {
			tmp = MidiConfig::getMode() - 7 + 'A';
			tmp += "С��ʽ";
		}
		std::cout << "��Ŀǰ�ĵ�ʽΪ: " << tmp << "   ��ɫΪ:" << Midi::InstrumentList[MidiConfig::getTimbre()].Chinese_name << "  ���Ϊ : " << MidiConfig::getVolume() << std::endl;
		std::cout << "��������9�˳�" << std::endl;
		std::cout << "A ~ K �ֱ��Ӧ do3 ~ do4" << std::endl;
		std::cout << "��������İ���:" << std::endl;
	}
}

// ��������
void Midi::playSound(HMIDIOUT handle, int frequency, int volume) {
	midiOutShortMsg(handle, frequency << 8 | 0x90 | volume << 16);

}

// ��ͣ����
void Midi::stopSound(HMIDIOUT handle, int frequency) {
	midiOutShortMsg(handle, frequency << 8 | 0x90 | 0 << 16);
}

// ����������Ϣ
void Midi::writeConfigFile() {
	MidiConfig::writeConfigFile();
}

// �˳������ٵ���
void Midi::quit() {
	Midi::KeyList.clear(); // �����ҵ�����
	Menu::setState(MENU); // ���ò˵���״̬Ϊ"���˵�"
	Menu::showMenu();
	Midi::flag = false; // �˳�����ʽ
}
