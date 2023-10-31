#include "Midi.h"

// ������Ƶ��(Hz)��Ӧ��
enum {
	A2 = 21, A2sharp = 22, B2 = 23,
	C1 = 24, C1sharp = 25, D1 = 26, D1sharp = 27, E1 = 28, F1 = 29, F1sharp = 30, G1 = 31, G1sharp = 32, A1 = 33, A1sharp = 34, B1 = 35,
	C = 36, Csharp = 37, D = 38, Dsharp = 39, E = 40, F = 41, Fsharp = 42, G = 43, Gsharp = 44, A = 45, Asharp = 46, B = 47,
	c = 48, csharp = 49, d = 50, dsharp = 51, e = 52, f = 53, fsharp = 54, g = 55, gsharp = 56, a = 57, asharp = 58, b = 59,
	c1 = 60, c1sharp = 61, d1 = 62, d1sharp = 63, e1 = 64, f1 = 65, f1sharp = 66, g1 = 67, g1sharp = 68, a1 = 69, a1sharp = 70, b1 = 71,
	c2 = 72, c2sharp = 73, d2 = 74, d2sharp = 75, e2 = 76, f2 = 77, f2sharp = 78, g2 = 79, g2sharp = 80, a2 = 81, a2sharp = 82, b2 = 83,
	c3 = 84, c3sharp = 85, d3 = 86, d3sharp = 87, e3 = 88, f3 = 89, f3sharp = 90, g3 = 91, g3sharp = 92, a3 = 93, a3sharp = 94, b3 = 95,
	c4 = 96, c4sharp = 97, d4 = 98, d4sharp = 99, e4 = 100, f4 = 101, f4sharp = 102, g4 = 103, g4sharp = 104, a4 = 105, a4sharp = 106, b4 = 107,
	c5 = 108
};

const int Midi::Major[7] = { 2,2,1,2,2,2,1 };
const int Midi::Minor[7] = { 2,1,2,2,1,2,2 };

const std::array<int, 14> Midi::MODE = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13 };

int Midi::mode = 0;
// MIDI���
HMIDIOUT Midi::handle;
// ��ɫ��
std::vector<InstrumentData> Midi::InstrumentList;

std::vector<int> Midi::SoundList;

std::vector<int> Midi::KeyList;

int Midi::getMode() {
	return Midi::mode;
}

void Midi::setMode(int mode) {
	Midi::mode = mode;
}


const std::array<int, 14>* Midi::getModeList() {
	return &Midi::MODE;
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

std::map<int, int> Midi::KeyMap;
void Midi::summonKeyMap() {
	std::string keys = "QWERTYUASDFGHJZXCVBNM";

	// ���ʽ
	if (Midi::mode <= 6 and Midi::mode >= 0) {
		Midi::KeyMap['Q'] = 45 + Midi::mode;// �趨��һ����
		for (int i = 1; i < keys.size(); i++) {
			Midi::KeyMap[keys[i]] = Midi::KeyMap[keys[i - 1]] + Midi::Major[(i - 1) % 7];
		}

	}
	// С��ʽ
	else {
		Midi::KeyMap['Q'] = 45 + Midi::mode - 7;// �趨��һ����,45ΪA��ID
		for (int i = 1; i < keys.size(); i++) {
			Midi::KeyMap[keys[i]] = Midi::KeyMap[keys[i - 1]] + Midi::Minor[(i - 1) % 7];
		}
	}
}

// ������ɫ
void  Midi::initialTimbre() {

	std::ifstream inputFile("timbre.txt");

	if (!inputFile.is_open()) {
		std::cerr << "����!�޷����ļ��Զ�ȡ��ɫ!" << std::endl;

	}
	else {
		std::string line;
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
// ��ȡ��ɫ��
std::vector<InstrumentData>* Midi::getInstrumentList() {
	return &Midi::InstrumentList;
}

// ���е�����
void Midi::runMIDI() {
	Midi::flag = true;
	Midi::showPrompts(); // ��ʾ��ʾ��
	midiOutShortMsg(Midi::handle, Midi::timbre << 8 | 0xC0);// �趨��ɫ
	// ���ɼ���������Ӧ��
	Midi::summonKeyMap();
	while (Midi::flag) {
		Midi::detectKeyboardInput(); // ����������
		Midi::operateKey(); // �Լ�����
	}
}

// ��������
void Midi::detectKeyboardInput() {
	Midi::KeyList.clear();
	for (int key = 1; key < 256; key++) {
		// ̽����Ƿ���
		if (GetKeyState(key) & 0x8000) {
			Midi::KeyList.push_back(key);
		}
	}
}

bool findVectorKey(std::vector<int>List, int value) {
	bool flag = false;
	for (auto i = List.begin(); i < List.end(); i++) {
		if (*i == value) flag = true;
	}
	return flag;
}

void deleteVectorKey(std::vector<int>&List, int key) {
	List.erase(std::remove(List.begin(), List.end(), key), List.end());
}


void Midi::operateKey() {


	for (int i : Midi::KeyList) {
		// ��������ļ�����Sound��,˵����һ�ΰ���,����
		if (not findVectorKey(SoundList, i) and Midi::KeyMap.count(i) > 0) {
			Midi::playSound(Midi::handle, Midi::KeyMap[i], Midi::volume);
			SoundList.push_back(i);
		}



		switch (i) {
		case '9':
			Midi::quit();
			break;

		case VK_NUMPAD9:
			Midi::quit();
			break;
		}
	}
	// ��Sound������������ڼ�����,˵���ɿ���,ͣ��
	for (int i : Midi::SoundList) {
		if (not findVectorKey(KeyList, i)) {
			Midi::playSound(Midi::handle, Midi::KeyMap[i], 0);
			deleteVectorKey(SoundList, i);
		}
	}
}

// ��ʼ��MIDI�����ٳ�ʽ
void Midi::initialMidi() {
	midiOutOpen(&Midi::handle, 0, 0, 0, CALLBACK_NULL);
	Midi::initialTimbre();
}
// ��ȡMIDI�����ٵ�ǰ����ɫ
int Midi::getTimbre() {
	return Midi::timbre;
}

// ��ȡMIDI�����ٵ�ǰ�����
int Midi::getVolume() {
	return Midi::volume;
}


// �趨��ɫ
void Midi::setTimbre(int id) {
	Midi::timbre = id;
}

// �趨���
void Midi::setVolume(int volume) {
	Midi::volume = volume;
}


// ��־�������Ƿ�����
bool Midi::flag = false;
int Midi::timbre = 0;
int Midi::volume = 100;

// ��ʾ��ʾ��
void Midi::showPrompts() {
	system("cls");
	std::string tmp;
	if (Midi::mode < 7) {
		tmp = mode + 'A';
		tmp += "���ʽ";
	}
	else {
		tmp = mode - 7 + 'A';
		tmp += "С��ʽ";
	}
	std::cout << "��Ŀǰ�ĵ�ʽΪ: " << tmp << "   ��ɫΪ:" << Midi::InstrumentList[Midi::timbre].Chinese_name << "  ���Ϊ : " << Midi::volume << std::endl;
	std::cout << "��������9�˳�" << std::endl;
	std::cout << "A ~ K �ֱ��Ӧ do3 ~ do4" << std::endl;
	std::cout << "��������İ���:" << std::endl;
}

// ��������
void Midi::playSound(HMIDIOUT handle, int frequency, int volume) {
	midiOutShortMsg(handle, frequency << 8 | 0x90 | volume << 16);

}

// ��ͣ����
void Midi::stopSound(HMIDIOUT handle, int frequency) {
	midiOutShortMsg(handle, frequency << 8 | 0x90 | 0 << 16);
}


// �˳������ٵ���
void Midi::quit() {
	Midi::KeyList.clear();
	Menu::setState(MENU);
	Menu::showMenu();
	Midi::flag = false;
}