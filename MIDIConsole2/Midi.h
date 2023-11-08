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

	static bool findVectorKey(std::vector<int>List, int value);
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
	static void setTick(int tick);
	static int getTick();
	
	
	static int getMode();
	static void setMode(int mode);
	// ����������Ϣ
	static void writeConfigFile();
	// ��ʾ��ʾ��Ϣ
	static void showCliPrompts();
private:
	// ����
	static const int MAJOR[7];
	static const int MINOR[7];
	static const std::array<int, 14> MODE;

	// ˽������

	static std::vector<int> RunSoundList;  // ���ڷ�����������
	static std::vector<int> StopSoundList;  // ����ֹͣ��������
	static std::vector<InstrumentData> InstrumentList;
	static std::vector<int> KeyList;  // 1tick�����¼��ļ�
	static std::vector<int> RealTimeKeyList;  // ʵʱ������¼��ļ�
	static std::vector<int> SoundList;
	static std::map<int, int>KeyMap;
	static int heart_beat;  // ÿtick����һ��,��ΧΪ0-tick
	static bool flag;
	static HMIDIOUT handle;

	// ����ҵ�ʵʱ������������
	static void operateRealTimeKey();

	// �����1tick����ļ�������Ӧ����
	static void operateKey();

	// ̽����ҵļ�����
	static void detectKeyboardInput();

	// ̽�����
	static void detectRealTimeKeyList();

	// ��ʼ����ɫ
	static void initialTimbre();

	// ��������
	static void playSound(HMIDIOUT handle, int frequency, int volume);

	// ֹͣ����
	static void stopSound(HMIDIOUT handle, int frequency);

	// ���ɼ��������Ķ�Ӧ��
	static void summonKeyMap();

	// �˳�midi������
	static void quit();

	// ��������
	static void disposeRest();
};

#endif // !MIDI_H