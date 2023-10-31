#include "Midi.h"

	const int Midi::Major[7] = { 2,2,1,2,2,2,1 };
	const int Midi::Minor[7] = { 2,1,2,2,1,2,2 };

	const std::array<int, 14> Midi::MODE = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13 };

	int Midi::mode = 0;

	// ����
	std::map<int, int> Midi::KeyMap;

	// MIDI���
	HMIDIOUT Midi::handle;

	// ��ɫ��
	std::vector<InstrumentData> Midi::InstrumentList;

	// 
	std::vector<int> Midi::SoundList;

	// �洢1tickʱ��Ұ��µļ���id
	std::vector<int> Midi::KeyList;

	// ��ȡMidi�ĵ�ʽ
	int Midi::getMode() {
		return Midi::mode;
	}

	// �趨Midi�ĵ�ʽ
	void Midi::setMode(int mode) {
		Midi::mode = mode;
	}

	// ��ȡ��ʽ�б�
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

	// ��ʼ��MIDI�����ٳ�ʽ
	void Midi::initialMidi() {
		midiOutOpen(&Midi::handle, 0, 0, 0, CALLBACK_NULL); // ����midi�豸
		Midi::initialTimbre();
	}

	// ���ɼ��������Ķ�Ӧ��
	void Midi::summonKeyMap() {
		// �趨���¿ɷ����ļ�
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
				// ����������ʽģʽ��ƥ��id,name��Chinese_name
				std::regex pattern(R"((\d+) (\w+) (\S+))");

				std::smatch match;

				if (std::regex_search(line, match, pattern)) {
					Midi::InstrumentList.push_back({ std::stoi(match[1]),name = match[2],Chinese_name = match[3] });
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
		Midi::showCliPrompts(); // ��ʾ��ʾ��
		midiOutShortMsg(Midi::handle, Midi::timbre << 8 | 0xC0);// �趨��ɫ
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

	// Ѱ��
	bool findVectorKey(std::vector<int>List, int value) {
		bool flag = false;
		for (auto i = List.begin(); i < List.end(); i++) {
			if (*i == value) flag = true;
		}
		return flag;
	}

	void deleteVectorKey(std::vector<int>& List, int key) {
		List.erase(std::remove(List.begin(), List.end(), key), List.end());
	}


	void Midi::operateKey() {


		for (int i : Midi::KeyList) {
			// ��������ļ�����Sound��,˵����һ�ΰ���,����
			if (not findVectorKey(SoundList, i) and Midi::KeyMap.count(i) > 0) {
				Midi::playSound(Midi::handle, Midi::KeyMap[i], Midi::volume);
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
	void Midi::showCliPrompts() {
		// �ж��Ƿ��������н�����
		if (_isatty(_fileno(stdout))) {
			system("cls");
			// �Ե�ʽ�����ı�����
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
		Midi::KeyList.clear(); // �����ҵ�����
		Menu::setState(MENU); // ���ò˵���״̬Ϊ"���˵�"
		Menu::showMenu();
		Midi::flag = false; // �˳�����ʽ
	}
