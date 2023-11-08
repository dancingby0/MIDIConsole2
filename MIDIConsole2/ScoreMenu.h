#pragma once



#include <string>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

#include "Midi.h"
#include "Menu.h"

struct ScoreMenuSound {
	int type;
	int time;
	int frequency;
	int volume;
	bool operator==(const ScoreMenuSound& other) const {
		return type == other.type and time == other.time and frequency == other.frequency and volume == other.volume;
	}
};

class ScoreMenu
{
public:
	static void runScore();
	static void showScore();
	static void ReadScoreFile();
	static bool findVectorKey(std::vector<std::string>List, std::string value);


private:
	

	static std::vector<ScoreMenuSound> SoundList;  // ������
	static int timbre;  // ����
	static std::string file_name; // ��������
	static int time;  // Ŀǰʱ��
	static int score_time;  // ������ʱ��
	static int file_count;  // �ļ�����
	static std::vector<std::string> ScoreList;
	static int page;
	static int id_pointer;
	

	// �Ե�ǰѡ�е��ļ�����
	static void disposeLine(std::string line);
	static void playScore();
	static void showPage();
	static void quitScoreMenu();
};

