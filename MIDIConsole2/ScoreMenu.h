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
	static int timbre;  // 音调
	static std::string file_name; // 谱子名称
	static int time;  // 目前时长
	static int score_time;  // 谱子总时长
	static int file_count;  // 文件总数
	static std::vector<std::string> ScoreList;
	static int page;
	static int id_pointer;



	
	static void showPage();
	static void quitScoreMenu();
};

