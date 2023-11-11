#include "StartAnimation.h"

void StartAnimation::runStartAnimation() {

	std::ifstream inputFile("Animation.txt");

	if (!inputFile.is_open()) {
		std::cerr << "无法打开文件: " << "Animation.txt" << std::endl;
		Sleep(1000);
		return;
	}

	std::string line;
	while (std::getline(inputFile, line)) {
		for (char c : line) {
			if (c == '0') {
				std::cout << "  ";  // 0对应两个空格
			}
			else if (c == '1') {
				std::cout << "**";  // 1对应"=="
			}
			else {
				std::cout << c;  // 其他字符原样输出
			}
		}
		std::cout << std::endl;
		Sleep(100);
	}
	Sleep(1000);

	inputFile.close();
}