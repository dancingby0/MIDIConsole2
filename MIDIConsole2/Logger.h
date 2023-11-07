#pragma once

/**
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

class Logger {
public:
    Logger(const std::string& logFileName) {
        logFile.open(logFileName, std::ios::app);
        if (logFile.is_open()) {
            log("Log started.");
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            log("Log ended.");
            logFile.close();
        }
    }

    void log(const std::string& message) {
        if (logFile.is_open()) {
            time_t now = std::time(nullptr);
            char timestamp[20];
            std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

            logFile << "[" << timestamp << "] " << message << std::endl;
            logFile.flush();
        }
    }

private:
    std::ofstream logFile;
};

int main() {
    Logger logger("mylog.txt");

    logger.log("This is an informational message.");
    logger.log("Another message.");
    logger.log("An error occurred.");

    return 0;
}
**/
class Logger
{


};