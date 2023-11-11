#include "StartAnimation.h"

void StartAnimation::runStartAnimation() {

	std::ifstream inputFile("Animation.txt");

	if (!inputFile.is_open()) {
		std::cerr << "�޷����ļ�: " << "Animation.txt" << std::endl;
		Sleep(1000);
		return;
	}

	std::string line;
	while (std::getline(inputFile, line)) {
		for (char c : line) {
			if (c == '0') {
				std::cout << "  ";  // 0��Ӧ�����ո�
			}
			else if (c == '1') {
				std::cout << "**";  // 1��Ӧ"=="
			}
			else {
				std::cout << c;  // �����ַ�ԭ�����
			}
		}
		std::cout << std::endl;
		Sleep(100);
	}
	Sleep(1000);

	inputFile.close();
}