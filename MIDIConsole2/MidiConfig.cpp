#include "MidiConfig.h"

// 初始化config属性
int MidiConfig::timbre;
int MidiConfig::volume;
int MidiConfig::mode;

// 提供音色
int MidiConfig::getTimbre() {
	return MidiConfig::timbre;
}

// 设置音色
void MidiConfig::setTimbre(int timbre) {
	MidiConfig::timbre = timbre;
}

// 提供响度
int MidiConfig::getVolume() {
	return MidiConfig::volume;
}

// 设置响度
void MidiConfig::setVolume(int volume) {
	MidiConfig::volume = volume;
}

// 提供调式
int MidiConfig::getMode() {
	return MidiConfig::mode;
}

// 设置调式
void MidiConfig::setMode(int mode) {
	MidiConfig::mode = mode;
}

// 读取配置文件
void MidiConfig::readConfigFile() {
	std::ifstream inputFile("config.txt");

	// 定义正则表达式模式
	std::regex pattern("timbre:(\\d+)\\nvolume:(\\d+)\\nmode:(\\d+)");

	std::string fileContents((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

	std::smatch match;

	if (std::regex_search(fileContents, match, pattern) and match.size() == 4) {

		MidiConfig::timbre = std::stoi(match[1]);
		MidiConfig::volume = std::stoi(match[2]);
		MidiConfig::mode = std::stoi(match[3]);

	}
}

// 写入配置信息
void MidiConfig::writeConfigFile() {
	// 打开 "config.txt" 以进行写入
	std::ofstream outputFile("config.txt");

	// 将更新后的值写入文件
	outputFile << "timbre:" << MidiConfig::timbre << "\n";
	outputFile << "volume:" << MidiConfig::volume << "\n";
	outputFile << "mode:" << MidiConfig::mode << "\n";
}