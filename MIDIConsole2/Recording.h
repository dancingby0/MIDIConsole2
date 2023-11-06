#pragma once

#include <Windows.h>
#include <iostream>
#include <direct.h>
#include <vector>

#include "Midi.h"

// 录制弹奏的MIDI琴并保存至文件(dat后缀)
class Recording
{
public:
	static const enum STATE { DONE, RECORDING, PAUSE };

	// 运行录制系统
	static void runRecording();
	// 开始录制
	static void startRecording();
	// 暂停录制
	static void switchPause();
	// 停止录制
	static void stopRecording();
	// 获取录制状态
	static int getState();
private:
	
	static int total_tick;
	static const double TICK;
	static std::vector<std::vector<int>> content;

	static int state;  // 录制的状态(0表未录制,1表正在录制)
	static std::string file_name;
};

