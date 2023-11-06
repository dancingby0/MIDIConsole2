#pragma once

#include <Windows.h>
#include <iostream>
#include <direct.h>
#include <vector>

#include "Midi.h"

// ¼�Ƶ����MIDI�ٲ��������ļ�(dat��׺)
class Recording
{
public:
	static const enum STATE { DONE, RECORDING, PAUSE };

	// ����¼��ϵͳ
	static void runRecording();
	// ��ʼ¼��
	static void startRecording();
	// ��ͣ¼��
	static void switchPause();
	// ֹͣ¼��
	static void stopRecording();
	// ��ȡ¼��״̬
	static int getState();
private:
	
	static int total_tick;
	static const double TICK;
	static std::vector<std::vector<int>> content;

	static int state;  // ¼�Ƶ�״̬(0��δ¼��,1������¼��)
	static std::string file_name;
};

