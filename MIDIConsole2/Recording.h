#pragma once

#include <Windows.h>
#include <iostream>
#include <direct.h>
#include <vector>
#include <filesystem>
#include <fstream>
#include "Midi.h"

// 录制弹奏的MIDI琴并保存至文件(dat后缀)
class Recording
{
public:
	const enum STATE { DONE, RECORDING, PAUSE };

	// 运行录制系统,每tick运行一次
	static void runRecordingTick();
	// 运行录制系统
	static void runRecording();

	// 获取录制状态
	static int getState();
private:

	static int total_time;
	static bool heart_beat;  // 录制状态的心跳,用于同MIDI电子琴同步
	static std::vector<std::vector<int>> content;

	static int state;  // 录制的状态(0表未录制,1表正在录制)
	static std::string file_name;

	static void newFile();
	static void writeFile();
	// 开始录制
	static void startRecording();
	// 暂停录制
	static void switchPause();
	// 停止录制
	static void stopRecording();

};

