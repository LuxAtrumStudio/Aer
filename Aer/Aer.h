#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

namespace AER {
	void GetWOEID(string location);
	void GetWeatherData(double lat, double lon, string location);
	void ProgramStartUp();
}