#pragma once



#include <string>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <filesystem>

#include "Midi.h"
#include "Menu.h"


class ScoreMenu
{
public:
	static void runScore();
	static void showScore();
	static void ReadScoreFile();
	static bool findVectorKey(std::vector<std::string>List, std::string value);
private:
	static int timbre;  // ����
	static std::string file_name; // ��������
	static int time;  // Ŀǰʱ��
	static int score_time;  // ������ʱ��
	static int file_count;  // �ļ�����
	static std::vector<std::string> ScoreList;
	static int page;
	static int id_pointer;



	
	static void showPage();
	static void quitScoreMenu();
};

