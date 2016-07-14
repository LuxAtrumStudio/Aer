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
	dataFile updateData = LUXLECTOR::LoadDataFile("ProgramData.lux");
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
			LOGGING::LogSuccess("Loaded new weather data for: " + updateData.data[3].stringVectorValue[a], "Aer.cpp/AER/ProgramStartUp");
		}
		updateData.data[0].intValue = currentTime;
		LUXLECTOR::SaveDataFile("ProgramData.lux", updateData);
	}
}