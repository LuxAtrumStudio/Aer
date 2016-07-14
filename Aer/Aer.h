#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

namespace AER {
	/*Data Loading*/
	void GetWOEID(string location);
	void GetWeatherData(double lat, double lon, string location);
	void ProgramStartUp();
	/*Program Setup*/
	void SetWindowLayout();
	void LoadJSONData();
	/*Displau Functions*/
	void DrawData();
	string GetData(tm date);
	string GetTime(tm date, bool meridies, bool seconeds);
	/*Change Funcions*/
	void LoadCurrentData();
	/*Run Functions*/
	void RunProgram();
}