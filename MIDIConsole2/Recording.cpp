#include "Recording.h"


int Recording::total_time = 0;
bool Recording::heart_beat = true;

std::string Recording::file_name = "";

int Recording::state = DONE;



// ��ȡ¼��״̬
int Recording::getState() {
	return Recording::state;
}

// ����¼��ϵͳ.ÿtick����һ��
void Recording::runRecordingTick() {

	// ����¼�Ƶ�״̬��������ͬʱ
	if (Recording::getState() == Recording::STATE::RECORDING) {
		total_time += Midi::getTick();  // ������ʱ��(�˴���λΪms)
		Recording::writeFile();
	}
}
// ����¼��ϵͳ,ÿ1ms����һ��
void Recording::runRecording() {


	for (int i : *Midi::getKeyList()) {

		// ����¼�Ƽ�ʱ
		if (i == 'P') {
			Sleep(200);  // ��ͣ200ms,�Է�ֹ��
			// ������¼��ʱ
			switch (Recording::getState()) {
				// ����¼��
			case Recording::STATE::DONE:
				Recording::startRecording();

				break;

				// ֹͣ¼��
			case Recording::STATE::PAUSE:
				Recording::stopRecording();
				break;

				//ֹͣ¼��
			case Recording::STATE::RECORDING:
				Recording::stopRecording();
				break;
			}
			// �������
			Midi::showCliPrompts();
		}

		// �����л���ͣ��ʱ
		else if (i == 'O' and not(Recording::getState() == Recording::STATE::DONE)) {
			Recording::switchPause();
			Sleep(200);
		}
	}
}


// ��ʼ¼��
void Recording::startRecording() {

	system("cls");
	Recording::newFile();

}


void Recording::switchPause() {

	if (Recording::state == STATE::RECORDING) {
		Recording::state = STATE::PAUSE;


	}
	else {
		Recording::state = STATE::RECORDING;

	}
	Midi::showCliPrompts();

}


void Recording::stopRecording() {
	// ��¼���ļ�
	std::ofstream outputFile("scores/" + Recording::file_name, std::ios::app);

	if (outputFile.is_open()) {
		// ���ļ���д��Ҫֹͣ�ļ�
		outputFile << "total_time: " << total_time << std::endl;

		outputFile.close();
	}
	else {
		std::cerr << "�޷����ļ� " << Recording::file_name << "��" << std::endl;
		Sleep(1000);
	}

	Recording::total_time = 0;
	Recording::file_name = "";
	Recording::state = STATE::DONE;
}



// �����µ�¼���ļ�
void Recording::newFile() {

	std::string folder = "scores";

	// ����ļ����Ƿ���ڣ�����������򴴽���
	if (not std::filesystem::is_directory(folder)) {
		std::filesystem::create_directory(folder);
	}

	bool flag_input = true;
	while (flag_input) {
		std::string input;
		
		
		system("cls");
		std::cout << "�����뱣���¼����Ƶ�ļ���(����\"QUIT\"ȡ��,����������ĸ������,��Ŀ¼�д�����ͬ���ļ���ᱻ����):" << std::endl;
		bool flag_input_0 = true;
		while (flag_input_0) {
			
			for (char key = 1; key < 256; key++) {
				// ̽����Ƿ񱻰���
				if (GetKeyState(key) & 0x8000 and (key <= 'z' and key >= 'a' or key <= 'Z' and key >= 'A' or key <= '9' and key >= '0')) {
					std::cout << key;
					input += key;
					Sleep(200);
				}
				if (GetKeyState(key) & 0x8000 and key == VK_RETURN) {
					flag_input_0 = false;
					break;
				}
				if (GetKeyState(key) & 0x8000 and key == VK_BACK and not input.empty()) {
					system("cls");
					std::cout << "�����뱣���¼����Ƶ�ļ���(����\":wq\"ȡ��,����������ĸ������,��Ŀ¼�д�����ͬ���ļ���ᱻ����):" << std::endl;
					input.pop_back();
					std::cout << input;
					Sleep(200);
				}
				
			}
		}
		bool flag = true;
		for (char i : input) {
			if (not(i <= 9 and i >= 0 or i <= 'z' and i >= 'a' or i <= 'Z' and i >= 'A')) {
				flag = false;
			}
		}
		if (input == "QUIT") {

			return;  // �˳�¼��
		}

		// �ļ������淶
		else if (flag) {
			Recording::file_name = input + ".scores";

			// ����ļ��Ƿ����
			if (std::filesystem::exists(folder + "/" + Recording::file_name)) {
				system("cls");
				std::cout << "�ļ� " << Recording::file_name << " �Ѵ���!" << std::endl;
				Recording::file_name = "";
				Sleep(1000);
			}
			else {
				// ���ļ�������ļ��������򴴽���,�������
				std::ofstream outputFile("scores/" + Recording::file_name, std::ios::ate);

				if (outputFile.is_open()) {
					// ���ļ���д���ʼ����
					outputFile << "timbre:" << Midi::getTimbre() << std::endl;
					outputFile << "volume:" << Midi::getVolume() << std::endl;
					outputFile << "mode:" << Midi::getMode() << std::endl;
					outputFile.close();
				}
				else {
					std::cerr << "�޷����ļ� " << Recording::file_name << "��" << std::endl;
					Sleep(1000);
				}
				flag_input = false;
				Recording::state = Recording::STATE::RECORDING;
				Sleep(100);
			}
		}
		// �ļ��������淶
		else {
			system("cls");
			std::cout << "�������˷Ƿ��ַ�!����������!" << std::endl;
			Sleep(1000);

		}
	}
}



void Recording::writeFile() {

	

	// ��ȡ�������б�
	for (int i : *Midi::getRunSoundList()) {
		// ��Run��Stop���ж�����ͬһ����,�򲻴洢
		if (Midi::findVectorKey(*Midi::getStopSoundList(), i)) {
			continue;
		}
		// ��¼���ļ�
		std::ofstream outputFile("scores/" + Recording::file_name, std::ios::app);

		if (outputFile.is_open()) {
			// ���ļ���д��Ҫ�����ļ�
			outputFile << "1 " << total_time << " " << i << std::endl;

			outputFile.close();
		}
		else {
			std::cerr << "�޷����ļ� " << Recording::file_name << "��" << std::endl;
			Sleep(1000);
		}
	}

	// ��ȡͣ�����б�
	for (int i : *Midi::getStopSoundList()) {
		if (Midi::findVectorKey(*Midi::getRunSoundList(), i)) {
			continue;
		}
		// ��¼���ļ�
		std::ofstream outputFile("scores/" + Recording::file_name, std::ios::app);

		if (outputFile.is_open()) {
			// ���ļ���д��Ҫֹͣ�ļ�
			outputFile << "0 " << total_time << " " << i << std::endl;

			outputFile.close();
		}
		else {
			std::cerr << "�޷����ļ� " << Recording::file_name << "��" << std::endl;
			Sleep(1000);
		}
	}

}