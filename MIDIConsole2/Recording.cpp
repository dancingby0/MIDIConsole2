#include "Recording.h"

const double Recording::TICK = 0.03125;
int Recording::total_tick = 0;


std::string Recording::file_name = "";

int Recording::state = DONE;


// ��ȡ¼��״̬
int Recording::getState() {
	return Recording::state;
}



// ����¼��ϵͳ
void Recording::runRecording() {
	for (int i : *Midi::getKeyList()) {

		if (i == 'P') {
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
		else if (i == 'O' and Recording::state == RECORDING) {
			// �л���ͣ״̬
			Recording::switchPause();
		}

	}


}
// ��ʼ¼��
void Recording::startRecording() {
	std::string input;
	system("cls");
	Sleep(200);  // ��ͣ200ms,�Է�ֹ��
	std::string folder = "./scores/";

	
	// ���ļ��в�����,����
	if (_access(folder.c_str(), 0))
	{
		//system("mkdir head");
		int success = _mkdir(folder.c_str());
	}

	bool flag_input = true;
	while (flag_input) {
		std::cout << "�����뱣���¼����Ƶ�ļ���(����\":wq\"ȡ��,����������ĸ������,��Ŀ¼�д�����ͬ���ļ���ᱻ����):" << std::endl;
		std::cin >> input;
		bool flag = true;
		for (char i : input) {
			if (not(i <= 9 and i >= 0 or i <= 'z' and i >= 'a' or i <= 'Z' and i >= 'A')) {
				flag = false;
			}
		}
		if (input == ":wq") {
			return;  // �˳�¼��
		}
		// �ļ������淶
		else if (flag) {
			flag_input = false;
			Recording::file_name = input;

		}
		// �ļ��������淶
		else {
			std::cout << "�������˷Ƿ��ַ�!����������!" << std::endl;
			Sleep(1000);
			system("cls");
		}
	}
	Recording::state = 1;
}


void Recording::switchPause() {
	if (Recording::state == STATE::RECORDING) {
		Recording::state = STATE::PAUSE;
	}
	else {
		Recording::state = STATE::RECORDING;
	}
	
}


void Recording::stopRecording() {
	Recording::state = STATE::DONE;
}