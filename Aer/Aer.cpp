#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <ctime>
#include <conio.h>
#include "Conscientia.h"
#include "Aer.h"

void AER::GetWOEID(string city)
{
	ofstream transfer("TransferData.txt");
	if (transfer.is_open()) {
		transfer << city;
		transfer.close();
	}
	system("GetWOEID.py");
}

void AER::GetWeatherData(int WOEID)
{
	ofstream transfer("TransferData.txt");
	if (transfer.is_open()) {
		transfer << 12795568;
		transfer.close();
	}
	system("GetData.py");
}