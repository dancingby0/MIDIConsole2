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

struct InstrumentData {
	int id;
	std::string name;
	std::string Chinese_name;
};

class Midi{
public:
	static void initialMidi();
	static void runMIDI();
	static int getTimbre();
	static int getVolume();
	static void setTimbre(int id);
	static void setVolume(int voice);
	static std::vector<InstrumentData>* getInstrumentList();
	static const std::array<int, 14>* getModeList();
	static int getMode();
	static void setMode(int mode);
	// ����������Ϣ
	static void writeConfigFile();
private:
	// ����
	static const int MAJOR[7];
	static const int MINOR[7];
	static const std::array<int, 14> MODE;

	// ˽������
	static std::vector<InstrumentData> InstrumentList;
	static std::vector<int> KeyList;
	static std::vector<int> SoundList;
	static std::map<int, int>KeyMap;
	static bool flag;
	static HMIDIOUT handle;


	// ����ҵ�����ļ�������Ӧ����
	static void operateKey();

	// ̽����ҵļ�����
	static void detectKeyboardInput();

	// ��ʼ����ɫ
	static void initialTimbre();

	// ��ʾ��ʾ��Ϣ
	static void showCliPrompts();

	// ��������
	static void playSound(HMIDIOUT handle, int frequency, int volume);

	// ֹͣ����
	static void stopSound(HMIDIOUT handle, int frequency);

	// ���ɼ��������Ķ�Ӧ��
	static void summonKeyMap();

	// �˳�midi������
	static void quit();
};

#endif // !MIDI_H