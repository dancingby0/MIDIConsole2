#pragma once
#ifndef MIDICONFIG_H
#define MIDICONFIG_H

#include <iostream>
#include <fstream>
#include <regex>
#include <string>

class MidiConfig{
public:
	// ��ȡ�����ļ�����ɫ
	static int getTimbre();

	// ������ɫ
	static void setTimbre(int);

	// ��ȡ�����ļ������
	static int getVolume();

	// �������
	static void setVolume(int);

	// ��ȡ�����ļ��ĵ�ʽ
	static int getMode();

	// ���õ�ʽ
	static void setMode(int);

	// ��ȡ��ȡ����С������
	static int getTick();

	// ���ö�ȡ����С������
	static void setTick(int);

	// ��ȡ�����ļ�
	static void readConfigFile();

	// �����������ļ�
	static void writeConfigFile();


private:
	static int timbre;  // ��ɫ
	static int volume;  // ���
	static int mode;  // ��ʽ
	static int tick;  // ʱ���



};

#endif // !MIDICONFIG_H