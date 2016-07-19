#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <ctime>
#include "Conscientia Headers.h"

using namespace std;
ofstream logFile;
bool logOpen = false;
void LOGGING::InitializeLogging() {
	logFile.open("Log.log");
	if (logFile.is_open()) {
		logOpen = true;
	}
	time_t currentTime;
	string dateTime;
	currentTime = time(0);
	dateTime = ctime(&currentTime);
	string temp = "";
	for (int a = 0; a < dateTime.size() - 1; a++) {
		temp = temp + dateTime[a];
	}
	dateTime = temp;
	string File = "Log Data.log";
	if (logOpen == true) {
		LogSuccess("Created New Log File", "Logging.cpp/initializeLogging");
		LogData(dateTime, "Consolas.cpp/initializeLogging");
	}
}
void LOGGING::LogError(string log, string location) {
	if (logOpen == true) {
		logFile << "[Error]:" + log + "[" + location + "]\n";
	}
}
void LOGGING::LogWarning(string log, string location) {
	if (logOpen == true) {
		logFile << "[Warning]:" + log + "[" + location + "]\n";
	}
}
void LOGGING::LogSuccess(string log, string location) {
	if (logOpen == true) {
		logFile << "[Success]:" + log + "[" + location + "]\n";
	}
}
void LOGGING::LogData(string log, string location) {
	if (logOpen == true) {
		logFile << "[Data]:" + log + "[" + location + "]\n";
	}
}
void LOGGING::Log(int code) {
	if (logOpen == true) {
		logFile << "[" + to_string(code) + "]" << "\n";
	}
}
void LOGGING::TerminateLogging() {
	LogSuccess("Terminated Log", "Logging.cpp/terminateLogging");
	logFile.close();
}
