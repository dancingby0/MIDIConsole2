#pragma once
#ifndef MIDI_H
#include <vector>
#include <array>
#include <string>
#include <Windows.h>
#include <map>
#pragma comment(lib,"winmm.lib")

// ��ɫ����
struct InstrumentData {
	int id;
	std::string name;
	std::string Chinese_name;
};

class Midi
{
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
private:
	// ��������
	static const int Major[7];
	static const int Minor[7];
	// ��ʽ��
	static const std::array<int, 14> MODE;
	// ��ʽ
	static int mode;
	// ��ɫ��
	static std::vector<InstrumentData> InstrumentList;
	// ��Ҽ���
	static std::vector<int> KeyList;
	// ����������
	static std::vector<int> SoundList;
	static bool flag;
	static int timbre;
	static int volume;
	static HMIDIOUT handle;
	static std::map<int, int>KeyMap;

	static void operateKey();
	static void detectKeyboardInput();
	static void initialTimbre();
	static void showPrompts();
	static void playSound(HMIDIOUT handle, int frequency, int volume);
	static void stopSound(HMIDIOUT handle, int frequency);
	static void summonKeyMap();
	static void quit();
};
#endif // !MIDI_H
