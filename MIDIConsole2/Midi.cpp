#include "Midi.h"



const int Midi::MAJOR[7] = { 2,2,1,2,2,2,1 };
const int Midi::MINOR[7] = { 2,1,2,2,1,2,2 };

const std::array<int, 14> Midi::MODE = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13 };

bool Midi::flag;

int Midi::heart_beat=0;

// ����
std::map<int, int> Midi::KeyMap;

// MIDI���
HMIDIOUT Midi::handle;

// �շ�����������
std::vector<int> Midi::RunSoundList;

// ��ͣ����������
std::vector<int> Midi::StopSoundList;

// ��ɫ��
std::vector<InstrumentData> Midi::InstrumentList;

// ���ڲ��ŵ�������
std::vector<int> Midi::SoundList;

// �洢1tickʱ��Ұ��µļ���id
std::vector<int> Midi::KeyList;

// �洢���ʵʱ���µļ���id
std::vector<int> Midi::RealTimeKeyList;

// �ж�vector���Ƿ����һ��ֵ

bool Midi::findVectorKey(std::vector<int>List, int value) {
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

 HMIDIOUT Midi::getHandle() {
	 return Midi::handle;
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

// ��ȡ���ڰ��µļ�
std::vector<int>* Midi::getKeyList() {
	return &Midi::RealTimeKeyList;
}

// ��ȡ���ڲ��ŵ�����
std::vector<int>* Midi::getSoundList() {
	return &Midi::SoundList;
}

// ��ȡ�շ���ʱ������
std::vector<int>* Midi::getRunSoundList() {
	return &Midi::RunSoundList;
}

// ��ȡ��ͣ��������
std::vector<int>* Midi::getStopSoundList() {
	return &Midi::StopSoundList;
}

// ��ȡMIDI�����ٵ�����
bool Midi::getHeartBeat() {
	return Midi::heart_beat;
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

// ��ȡʱ���
int Midi::getTick() {
	return MidiConfig::getTick();
}

// �趨ʱ���
void Midi::setTick(int tick) {
	MidiConfig::setTick(tick);
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
			int state = 0;// ��ȡһ��ʱ��ʼ״̬(��ȡid)

			for (int i = 0; i < line.size(); i++) {

				// ��id�����ַ�Χ�ڲ���ʱ״̬0ʱ,��ȡ
				if ('0' <= line[i] and line[i] <= '9' and state == 0) {
					id = id * 10 + (line[i] - '0');
				}
				// ����������(�ո�),��״̬λ��0(��ȡ����ʱ),��ת��Ϊ1(��ȡname)
				else if (state == 0) {
					state = 1;
				}
				// ��״̬Ϊname,��Ϊ ��ĸ �� �ո� �� ������ �� С���� �� ���� ʱ��Ϊname
				if (state == 1 // ״̬1
					and ('A' <= line[i] and 'Z' >= line[i] or 'a' <= line[i] and 'z' >= line[i]  // ��ĸ
						or line[i] == ' ' or line[i] == '(' or line[i] == ')' or line[i] == '+' or line[i] == '-'  // ��������
						or line[i] <= '9' and '0' <= line[i])) {
					name += line[i];
				}
				// ����(��״̬Ϊ1),�����������ַ�(����),״̬Ϊ2(��ȡ����)
				else if (state == 1) {
					state = 2;
				}
				// ��״̬Ϊ2,���ȡ����
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
	Midi::showCliPrompts();  // ��ʾ��ʾ��
	midiOutShortMsg(Midi::handle, MidiConfig::getTimbre() << 8 | 0xC0);  // �趨��ɫ
	Midi::summonKeyMap();// ���ɼ���������Ӧ��
	Midi::heart_beat = 0;


	// ���ѭ��,1msѭ��һ��,�ܹ�ʱʱ�̻̿�ȡ��
	// �ڲ㷢��ѭ��,��ѭtick����,�ܹ��淶�����ķ���.
	while (Midi::flag) {

		Midi::detectKeyboardInput();  // ������ʵʱ����

		Midi::operateRealTimeKey();  // �Լ�����(ʵʱ����)

		// �˴�������ӹ���
		Recording::runRecording();  // ����¼��ϵͳ


		Midi::disposeRest();
		heart_beat++;
		Sleep(1);  // ���ѭ��1msһ��
	}
}

// ������ʵʱ����ļ�
void Midi::detectKeyboardInput() {
	for (int key = 1; key < 256; key++) {
		// ̽����Ƿ���(��16λΪ���µ��ж�λ)
		if (GetKeyState(key) & 0x8000) {
			// ����ʵʱ���ı�
			Midi::RealTimeKeyList.push_back(key);
		}
	}

	for (int i : Midi::RealTimeKeyList) {
		// ��1tick�ļ������Ҳ���ʵʱ���ı�����ҵ�����,����ȥ
		if (not findVectorKey(KeyList, i)) {
			Midi::KeyList.push_back(i);
		}
	}
}

// ��������
// ��heart_beat��tick���ʱ,����1tick���е��¼�
void Midi::disposeRest() {
	Midi::operateKey();  //ִ��һ��tickʱ������Ӧ���� ����,ͣ��
	Midi::RealTimeKeyList.clear(); // ����ȡ��һ��ʵʱ����ʱ,�����Ҽ���
	

	// 1tick����,����1tick�������¼�
	if (Midi::heart_beat >= MidiConfig::getTick()) {
		

		// �������1tickʱҪ���е���������(��ȡ�������ͣ����)
		Recording::runRecordingTick();

		Midi::heart_beat = 0;  // ��ʼ�µ�һ������
		Midi::KeyList.clear(); // ����ȡ��1tick����ʱ,�����Ҽ���
		// ��շ�����,ͣ����
		Midi::RunSoundList.clear();
		Midi::StopSoundList.clear();
	}
}

// ����ʵʱ������
void Midi::detectRealTimeKeyList() {
	
	for (int i : Midi::RealTimeKeyList) {
		// ��1tick�ļ������Ҳ���ʵʱ���ı�����ҵ�����,����ȥ
		if (not findVectorKey(KeyList, i)) {
			Midi::KeyList.push_back(i);
		}
	}
}

// �������ʵʱ���µļ�ִ�в���
// ִ�� : �˳�
void Midi::operateRealTimeKey() {
	for (int i : Midi::RealTimeKeyList) {
		
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
}

// �����1tick����ļ�������Ӧ����
// ִ�� : ����,ͣ��
void Midi::operateKey() {
	for (int i : Midi::RealTimeKeyList) {
		// ��������ļ�����Sound��,˵����һ�ΰ���,����
		if (not findVectorKey(Midi::SoundList, i) and Midi::KeyMap.count(i) > 0) {
			Midi::playSound(Midi::handle, Midi::KeyMap[i], MidiConfig::getVolume());
			
			if (not findVectorKey(Midi::RunSoundList, i)){
				Midi::RunSoundList.push_back(KeyMap[i]);  // ��1tick�ڷ�������������RunSoundList��
			}
			Midi::SoundList.push_back(i);
		}
	}
	// ��Sound������������ڼ�����,˵���ɿ���,ͣ��
	for (int i : Midi::SoundList) {
		if (not findVectorKey(Midi::RealTimeKeyList, i)) {
			Midi::playSound(Midi::handle, Midi::KeyMap[i], 0);
			if (not findVectorKey(Midi::StopSoundList, KeyMap[i])) {
				Midi::StopSoundList.push_back(KeyMap[i]);  // ��1tick��ͣ������������StopSoundList��
			}
			[](std::vector<int>& List, int key) {List.erase(std::remove(List.begin(), List.end(), key), List.end()); }(Midi::SoundList, i);
		}
	}
}

// ��ʾ��ʾ��(CLI)
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
		// ������¼��
		if (Recording::getState()) {
			std::cout << "����O��ͣ¼��,����P��ͣ¼��" << std::endl;
		}
		else {
			std::cout << "����P����¼�Ƶ��������" << std::endl;
		}
		std::cout << "��������İ���:" << std::endl;
		// ����¼��ʱ
		switch (Recording::getState()) {
		case Recording::STATE::PAUSE:
			std::cout << "��ͣ¼��" << std::endl;
			break;
		case Recording::STATE::RECORDING:
			std::cout << "����¼����" << std::endl;
			break;
		}
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
