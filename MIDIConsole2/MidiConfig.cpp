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
	std::ifstream inputFile("config.txt");

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

// д��������Ϣ
void MidiConfig::writeConfigFile() {
	// �� "config.txt" �Խ���д��
	std::ofstream outputFile("config.txt");

	// �����º��ֵд���ļ�
	outputFile << "timbre:" << MidiConfig::timbre << "\n";
	outputFile << "volume:" << MidiConfig::volume << "\n";
	outputFile << "mode:" << MidiConfig::mode << "\n";
}