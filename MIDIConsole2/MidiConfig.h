#pragma once
#ifndef MIDICONFIG_H
#define MIDICONFIG_H

#include <iostream>
#include <fstream>
#include <regex>
#include <string>

class MidiConfig{
public:
	// 获取配置文件的音色
	static int getTimbre();

	// 设置音色
	static void setTimbre(int);

	// 获取配置文件的响度
	static int getVolume();

	// 设置响度
	static void setVolume(int);

	// 获取配置文件的调式
	static int getMode();

	// 设置调式
	static void setMode(int);

	// 获取读取的最小节拍数
	static int getTick();

	// 设置读取的最小节拍数
	static void setTick(int);

	// 读取配置文件
	static void readConfigFile();

	// 保存至配置文件
	static void writeConfigFile();


private:
	static int timbre;  // 音色
	static int volume;  // 响度
	static int mode;  // 调式
	static int tick;  // 时间刻



};

#endif // !MIDICONFIG_H