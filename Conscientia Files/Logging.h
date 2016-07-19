#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;
namespace LOGGING {
	void InitializeLogging();
	void LogError(string log, string location);
	void LogWarning(string log, string location);
	void LogSuccess(string log, string location);
	void LogData(string log, string location);
	void Log(int code);
	void TerminateLogging();
}
