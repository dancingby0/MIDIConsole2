#pragma once
#ifndef MIDI_H
#define MIDI_H

#include <vector>
#include <array>
#include <string>
#include <Windows.h>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <io.h>
#include <regex>
#pragma comment(lib,"winmm.lib")

#include"MidiConfig.h"
#include "Menu.h"
#include "Recording.h"

struct InstrumentData {
	int id;
	std::string name;
	std::string Chinese_name;
};

class Midi{
public:
	static const double TICK;

	static void initialMidi();
	static void runMIDI();
	static int getTimbre();
	static int getVolume();
	static bool getHeartBeat();
	static std::vector<int>* getKeyList();
	static std::vector<int>* getSoundList();
	static std::vector<InstrumentData>* getInstrumentList();
	static std::vector<int>* getRunSoundList();
	static std::vector<int>* getStopSoundList();
	static const std::array<int, 14>* getModeList();
	static void setTimbre(int id);
	static void setVolume(int voice);
	
	
	static int getMode();
	static void setMode(int mode);
	// 保存配置信息
	static void writeConfigFile();
	// 显示提示信息
	static void showCliPrompts();
private:
	// 常量
	static const int MAJOR[7];
	static const int MINOR[7];
	static const std::array<int, 14> MODE;

	// 私有属性

	static std::vector<int> RunSoundList;  // 正在发声的音符表
	static std::vector<int> StopSoundList;  // 正在停止的音符表
	static std::vector<InstrumentData> InstrumentList;
	static std::vector<int> KeyList;
	static std::vector<int> SoundList;
	static std::map<int, int>KeyMap;
	static bool hear_beat;
	static bool flag;
	static HMIDIOUT handle;


	// 对玩家的输入的键作出相应操作
	static void operateKey();

	// 探测玩家的键输入
	static void detectKeyboardInput();

	// 初始化音色
	static void initialTimbre();

	// 播放声音
	static void playSound(HMIDIOUT handle, int frequency, int volume);

	// 停止声音
	static void stopSound(HMIDIOUT handle, int frequency);

	// 生成键与音调的对应表
	static void summonKeyMap();

	// 退出midi电子琴
	static void quit();
};

#endif // !MIDI_H