#include "MidiConfig.h"

// ��ʼ��config����
int MidiConfig::timbre;
int MidiConfig::volume;
int MidiConfig::mode;

// �ṩ��ɫ
int MidiConfig::getTimbre() {
	return MidiConfig::timbre;
}

// ������ɫ
void MidiConfig::setTimbre(int timbre) {
	MidiConfig::timbre = timbre;
}

// �ṩ���
int MidiConfig::getVolume() {
	return MidiConfig::volume;
}

// �������
void MidiConfig::setVolume(int volume) {
	MidiConfig::volume = volume;
}

// �ṩ��ʽ
int MidiConfig::getMode() {
	return MidiConfig::mode;
}

// ���õ�ʽ
void MidiConfig::setMode(int mode) {
	MidiConfig::mode = mode;
}


// ��ȡ�����ļ�
void MidiConfig::readConfigFile() {
	std::string file_name = "config.txt";
	
	std::ifstream inputFile(file_name);

	if (not inputFile) {
		inputFile.close();
		// ���ļ�������ļ��������򴴽���
		std::ofstream outputFile(file_name, std::ios::out | std::ios::trunc);
		outputFile << "timbre:0\nvolume:100\nmode:0\n";
		MidiConfig::timbre = 0;
		MidiConfig::volume = 100;
		MidiConfig::mode = 0;
		outputFile.close();
	}
	else {
		// ����������ʽģʽ
		std::regex pattern("timbre:(\\d+)\\nvolume:(\\d+)\\nmode:(\\d+)");

		std::string fileContents((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

		std::smatch match;

		if (std::regex_search(fileContents, match, pattern) and match.size() == 4) {

			MidiConfig::timbre = std::stoi(match[1]);
			MidiConfig::volume = std::stoi(match[2]);
			MidiConfig::mode = std::stoi(match[3]);

		}
	}
}

// д��������Ϣ
void MidiConfig::writeConfigFile() {
	// �� "config.txt" �Խ���д��
	std::ofstream outputFile("config.txt");

	// �����º��ֵд���ļ�
	outputFile << "timbre:" << MidiConfig::timbre << "\n";
	outputFile << "volume:" << MidiConfig::volume << "\n";
	outputFile << "mode:" << MidiConfig::mode << "\n";
}