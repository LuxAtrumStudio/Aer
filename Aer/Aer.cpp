#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <ctime>
#include <conio.h>
#include "Conscientia Files\Conscientia Headers.h"
#include "Aer.h"
using namespace std;

int currentLocation = 0;
bool day = true;
dataFile updateData;
string locationData, weatherData;

void AER::GetWOEID(string location)
{
	vector<string> scriptData = {
		"", "http://where.yahooapis.com/v1/places.q(" + location + ")?appid=[dj0yJmk9T2tDdFJ3bHlBa0ZyJmQ9WVdrOVdHdGlUemxWTnpBbWNHbzlNQS0tJnM9Y29uc3VtZXJzZWNyZXQmeD1kOQ--]" + "&format=json", "WOEID/" + location + "WOEIDData.json"
	};
	CONCERO::RunScript(scriptData);
}

void AER::GetWeatherData(double lat, double lon, string location)
{
	vector<string> scriptData = {
		"", "https://api.forecast.io/forecast/9c9c48f9ab5717ed899e8b6d730883c6/" + to_string(lat) + "," + to_string(lon), "WeatherData/" + location + "WeatherData.json"
	};
	CONCERO::RunScript(scriptData);
}

void AER::ProgramStartUp()
{
	updateData = LUXLECTOR::LoadDataFile("ProgramData.lux");
	time_t currentTime, lastUpdate;
	lastUpdate = (time_t)updateData.data[0].intValue;
	time(&currentTime);
	tm currentTM;
	tm lastUpdateTM;
	currentTM = *gmtime(&currentTime);
	lastUpdateTM = *gmtime(&lastUpdate);
	if (currentTM.tm_hour != lastUpdateTM.tm_hour ||
		currentTM.tm_mday != lastUpdateTM.tm_mday ||
		currentTM.tm_mon != lastUpdateTM.tm_mon ||
		currentTM.tm_year != lastUpdateTM.tm_year) {
		cout << ">Fetching New Location Data:\n";
		for (unsigned a = 0; a < updateData.data[3].stringVectorValue.size(); a++) {
			cout << ">>" + updateData.data[3].stringVectorValue[a] << endl;
			GetWOEID(updateData.data[3].stringVectorValue[a]);
			string fileName = "WOEID/" + updateData.data[3].stringVectorValue[a] + "WOEIDData.json";
			CONCERO::InterpretJSON(fileName);
			JSONFile newWOEID = CONCERO::GetJSONFile(fileName);
			subVar centriod = CONCERO::GetVariable("centroid", fileName);
			double lat, lon;
			lat = centriod.vars[0].doubleVar;
			lon = centriod.vars[1].doubleVar;
			updateData.data[1].stringVectorValue.push_back(updateData.data[3].stringVectorValue[a]);
			updateData.data[2].doubleVectorValue.push_back(lat);
			updateData.data[2].doubleVectorValue.push_back(lon);
			LOGGING::LogSuccess("Loaded new location data for: " + updateData.data[3].stringVectorValue[a], "Aer.cpp/AER/ProgramStartUp");
		}
		updateData.data[3].stringVectorValue.clear();
		cout << ">Fetching Updated Weather Data:\n";
		for (unsigned a = 0; a < updateData.data[1].stringVectorValue.size(); a++) {
			double lat, lon;
			lat = updateData.data[2].doubleVectorValue[a * 2];
			lon = updateData.data[2].doubleVectorValue[(a * 2) + 1];
			cout << ">>" << updateData.data[1].stringVectorValue[a] << ":" << lat << "," << lon << endl;
			GetWeatherData(lat, lon, updateData.data[1].stringVectorValue[a]);
			LOGGING::LogSuccess("Loaded new weather data for: " + updateData.data[1].stringVectorValue[a], "Aer.cpp/AER/ProgramStartUp");
		}
		updateData.data[0].intValue = currentTime;
		LUXLECTOR::SaveDataFile("ProgramData.lux", updateData);
	}
	LoadJSONData();
}

void AER::SetWindowLayout()
{
	int maxX = 0, maxY = 0;
	/*Menu Bar*/
	CONSCIENTIA::GetWindowSize(0, maxX, maxY);
	CONSCIENTIA::GenorateWindow("currentTemp", 0, 0, 12, 3, true, false);
	CONSCIENTIA::GenorateWindow("locationData", 12, 0, (maxX - 52), 3, true, false);
	CONSCIENTIA::GenorateWindow("dateTime", (maxX - 40), 0, 40, 3, true, false);
	/*Core Windows*/
	CONSCIENTIA::GenorateWindow("Current Weather Data", 0, 3, (maxX / 10) * 3, maxY - 3, true, true);
	CONSCIENTIA::GenorateWindow("Forcast Selection", (maxX / 10) * 3, 3, (maxX / 10) * 2, maxY - 3, true, true);
	CONSCIENTIA::GenorateWindow("Forcasted Weather Data", (maxX / 10) * 5, 3, (maxX / 10) * 5, maxY - 3, true, true);
}

void AER::LoadJSONData()
{
	for (unsigned a = 0; a < updateData.data[1].stringVectorValue.size(); a++) {
		CONCERO::InterpretJSON("WOEID/" + updateData.data[1].stringVectorValue[a] + "WOEIDData.json");
		CONCERO::InterpretJSON("WeatherData/" + updateData.data[1].stringVectorValue[a] + "WeatherData.json");
	}
}

void AER::DrawData()
{
	int tempSizeX, tempSizeY;
	/*Date Time*/
	time_t currentTime;
	time(&currentTime);
	tm curretnTM = *localtime(&currentTime);
	string dateTime = GetData(curretnTM) + " " + GetTime(curretnTM, true, false);
	CONSCIENTIA::FGetWindowSize("dateTime", tempSizeX, tempSizeY);
	CONSCIENTIA::FMPrint("dateTime", CONSCIENTIA::FindTextStart(dateTime, tempSizeX), 1, dateTime);
	/*Location*/
	string location = "";
	location = CONCERO::GetStringVariable("name", locationData) + ", ";
	location = location + CONCERO::GetStringVariable("admin1", locationData) + ", ";
	location = location + CONCERO::GetStringVariable("country", locationData);
	CONSCIENTIA::FGetWindowSize("locationData", tempSizeX, tempSizeY);
	CONSCIENTIA::FMPrint("locationData", CONSCIENTIA::FindTextStart(location, tempSizeX), 1, location);
	/*CurrentTemp Menu*/
	subVar currentData = CONCERO::GetVariable("currently", weatherData);
	string currentTemp = to_string(currentData.vars[7].doubleVar);
	string tempTemp;
	int a = 0, b = 0;
	for (a = 0; a < currentTemp.size(); a++) {
		tempTemp = tempTemp + currentTemp[a];
		if (currentTemp[a] == '.') {
			tempTemp = tempTemp + currentTemp[a + 1] + currentTemp[a + 2];
			break;
		}
	}
	currentTemp = tempTemp;
	currentTemp = currentTemp + char(248) + "F";
	CONSCIENTIA::FGetWindowSize("currentTemp", tempSizeX, tempSizeY);
	CONSCIENTIA::FMPrint("currentTemp", CONSCIENTIA::FindTextStart(currentTemp, tempSizeX), 1, currentTemp);

	/*>>>>>CORE DATA<<<<<*/
	/*Current Data*/
	string line;
	for (unsigned a = 0; a < currentData.vars.size(); a++) {
		line = "";
	}
	/*Update*/
	CONSCIENTIA::DrawBorder(1);
	CONSCIENTIA::DrawBorder(2);
	CONSCIENTIA::DrawBorder(3);
	CONSCIENTIA::DrawBorder(4);
	CONSCIENTIA::DrawBorder(5);
	CONSCIENTIA::DrawBorder(6);
	CONSCIENTIA::DrawTitle(4);
	CONSCIENTIA::DrawTitle(5);
	CONSCIENTIA::DrawTitle(6);
}

string AER::GetData(tm date)
{
	string dateString = "";
	string wDays[7] = {
		"Sunday",
		"Monday",
		"Tuesday",
		"Wensday",
		"Thrusday",
		"Friday",
		"Saturday"
	};
	string months[12] = {
		"January",
		"Febuary",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"
	};
	dateString = wDays[date.tm_wday] + " " + months[date.tm_mon] + " " + to_string(date.tm_mday) + " " + to_string(1900 + date.tm_year);
	return(dateString);
}

string AER::GetTime(tm date, bool meridies, bool seconeds)
{
	string time;
	bool pm = false;
	if (meridies == true && date.tm_hour > 12) {
		date.tm_hour = date.tm_hour - 12;
		pm = true;
	}
	time = to_string(date.tm_hour) + ":";
	if (date.tm_min < 10) {
		time = time + "0" + to_string(date.tm_min);
	}
	else {
		time = time + to_string(date.tm_min);
	}
	if (seconeds == true) {
		time = time + ":" + to_string(date.tm_sec);
	}
	if (meridies == true) {
		if (pm == false) {
			time = time + " AM";
		}
		if (pm == true) {
			time = time + " PM";
		}
	}
	return(time);
}

void AER::LoadCurrentData()
{
	locationData = "WOEID/" + updateData.data[1].stringVectorValue[currentLocation] + "WOEIDData.json";
	weatherData = "WeatherData/" + updateData.data[1].stringVectorValue[currentLocation] + "WeatherData.json";
}

void AER::RunProgram()
{
	bool running = true;
	int input = -1;
	SetWindowLayout();
	LoadCurrentData();
	while (running == true) {
		DrawData();
		CONSCIENTIA::Update();
		input = CONSCIENTIA::Gint();
		if (input == 27) {
			running = false;
		}
	}
}