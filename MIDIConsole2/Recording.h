#pragma once

#include <Windows.h>
#include <iostream>
#include <direct.h>
#include <vector>
#include <filesystem>
#include <fstream>
#include "Midi.h"

// ¼�Ƶ����MIDI�ٲ��������ļ�(dat��׺)
class Recording
{
public:
	const enum STATE { DONE, RECORDING, PAUSE };

	// ����¼��ϵͳ,ÿtick����һ��
	static void runRecordingTick();
	// ����¼��ϵͳ
	static void runRecording();

	// ��ȡ¼��״̬
	static int getState();
private:

	static int total_time;
	static bool heart_beat;  // ¼��״̬������,����ͬMIDI������ͬ��
	static std::vector<std::vector<int>> content;

	static int state;  // ¼�Ƶ�״̬(0��δ¼��,1������¼��)
	static std::string file_name;

	static void newFile();
	static void writeFile();
	// ��ʼ¼��
	static void startRecording();
	// ��ͣ¼��
	static void switchPause();
	// ֹͣ¼��
	static void stopRecording();

};

