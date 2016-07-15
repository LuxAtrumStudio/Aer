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
int currentForcast = 0, maxForcast = 0;
int displayStartCurrent = 0, displayStartForcast = 0;

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
		"", "https://api.forecast.io/forecast/9c9c48f9ab5717ed899e8b6d730883c6/" + to_string(lat) + "," + to_string(lon) + "?exclude=[minutely]", "WeatherData/" + location + "WeatherData.json"
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
		currentTM.tm_year != lastUpdateTM.tm_year || updateData.data[3].stringVectorValue.size() > 0) {
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
	CONSCIENTIA::GenorateWindow("Current Weather Data", 0, 3, ((maxX / 10) * 4) - 1, maxY - 3, true, true);
	CONSCIENTIA::GenorateWindow("Forcast Selection", ((maxX / 10) * 4) - 1, 3, ((maxX / 10) * 2) + 1, maxY - 3, true, true);
	CONSCIENTIA::GenorateWindow("Forcasted Weather Data", (maxX / 10) * 6, 3, (maxX / 10) * 4, maxY - 3, true, true);
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
	int offset = CONCERO::GetIntVariable("offset", weatherData);
	currentTime = currentTime + (offset * 3600);
	tm currentTM = *gmtime(&currentTime);
	string dateTime = GetData(currentTM) + " " + GetTime(currentTM, true, false);
	CONSCIENTIA::FGetWindowSize("dateTime", tempSizeX, tempSizeY);
	CONSCIENTIA::FMPrint("dateTime", CONSCIENTIA::FindTextStart(dateTime, tempSizeX), 1, dateTime);
	/*Location*/
	subVar currentData = CONCERO::GetVariable("currently", weatherData);
	string location = "";
	location = CONCERO::GetStringVariable("name", locationData) + ", ";
	location = location + CONCERO::GetStringVariable("admin1", locationData) + ", ";
	location = location + CONCERO::GetStringVariable("country", locationData);
	CONSCIENTIA::FGetWindowSize("locationData", tempSizeX, tempSizeY);
	CONSCIENTIA::FMPrint("locationData", CONSCIENTIA::FindTextStart(location, tempSizeX), 1, location);
	/*CurrentTemp Menu*/
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
	int lineNumber;
	/*Current Data*/
	string line;
	lineNumber = 1;
	for (unsigned a = displayStartCurrent; a < currentData.vars.size(); a++) {
		line = "";
		line = ConvertVartName(currentData.vars[a].name) + ": " + ConvertVar(currentData.vars[a]);
		CONSCIENTIA::FMPrint("Current Weather Data", 1, lineNumber, line);
		lineNumber++;
	}
	/*Forcasted Data*/
	subVar selectedType;
	subVar selectedData;
	if (day == true) {
		selectedType = CONCERO::GetVariable("daily", weatherData);
		selectedData = selectedType.vars[2 + currentForcast];
		maxForcast = selectedType.vars.size() - 3;
	}
	else {
		selectedType = CONCERO::GetVariable("hourly", weatherData);
		selectedData = selectedType.vars[2 + currentForcast];
		maxForcast = selectedType.vars.size() - 3;
	}
	lineNumber = 1;
	for (unsigned a = displayStartForcast; a < selectedData.vars.size(); a++) {
		line = "";
		line = ConvertVartName(selectedData.vars[a].name) + ": " + ConvertVar(selectedData.vars[a]);
		CONSCIENTIA::FMPrint("Forcasted Weather Data", 1, lineNumber, line);
		lineNumber++;
	}
	/*Selection Data*/
	if (day == true) {
		CONSCIENTIA::FGetWindowSize("Forcast Selection", tempSizeX, tempSizeY);
		CONSCIENTIA::FMPrint("Forcast Selection", CONSCIENTIA::FindTextStart("7 Day Forcast", tempSizeX), 1, "7 Day Forcast");
	}
	else {
		CONSCIENTIA::FGetWindowSize("Forcast Selection", tempSizeX, tempSizeY);
		CONSCIENTIA::FMPrint("Forcast Selection", CONSCIENTIA::FindTextStart("48 Hour Forcast", tempSizeX), 1, "48 Hour Forcast");
	}
	int displayStart = 0;
	while (currentForcast > tempSizeY + displayStart - 6) {
		displayStart++;
	}
	lineNumber = 3;
	for (unsigned a = displayStart; a < selectedType.vars.size() - 2 && a < tempSizeY + displayStart - 2; a++) {
		line = "";
		time_t forcastTime = (time_t)selectedType.vars[2 + a].vars[0].intVar;
		int offset = CONCERO::GetIntVariable("offset", weatherData);
		forcastTime = forcastTime + (offset * 3600);
		tm forcastTM = *gmtime(&forcastTime);
		if (day == true) {
			line = GetData(forcastTM);
		}
		if (day == false) {
			line = GetTime(forcastTM, true, false);
		}
		if (a == currentForcast) {
			line = ">" + line + "<";
		}
		CONSCIENTIA::FMPrint("Forcast Selection", CONSCIENTIA::FindTextStart(line, tempSizeX), lineNumber, line);
		lineNumber++;
	}
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

string AER::ConvertVartName(string var)
{
	string name;
	name = char(int(var[0]) - 32);
	for (unsigned a = 1; a < var.size() - 1; a++) {
		if (int(var[a]) < 97) {
			name = name + ' ';
		}
		if (var[a] != '-') {
			name = name + var[a];
		}
	}
	name = name + var[var.size() - 1];
	return(name);
}

string AER::ConvertVar(subVar var)
{
	string variable = "";
	if (var.name == "time") {
		time_t dataTime = (time_t)var.intVar;
		int offset = CONCERO::GetIntVariable("offset", weatherData);
		dataTime = dataTime + (offset * 3600);
		tm dataTM = *gmtime(&dataTime);
		variable = GetData(dataTM) + " " + GetTime(dataTM, true, true);
	}
	else if (var.name == "nearestStormDistance" || var.name == "visibility") {
		variable = to_string(var.intVar) + " mi";
	}
	else if (var.name == "nearestStormBearing" || var.name == "windBearing") {
		string bearings[16] = { "North",
			"North, North East",
			"North East",
			"East, North East",
			"East",
			"East, South East",
			"South East",
			"South, South East",
			"South",
			"South, South West",
			"South West",
			"West, South West",
			"West",
			"West, North West",
			"North West",
			"North, North West"
		};
		int bearing = (int)((var.intVar / 22.5) + 0.5);
		variable = bearings[bearing % 16];
	}
	else if (var.name == "precipIntensity" || var.name == "precipIntensityMax") {
		variable = var.strVar + " in/hr";
	}
	else if (var.name == "precipProbability" || var.name == "humidity" || var.name == "cloudCover" || var.name == "moonPhase") {
		if (var.strVar == "0") {
			variable = "0%";
		}
		else {
			variable = to_string((var.doubleVar * (double)100)) + "%";
		}
	}
	else if (var.name == "temperature" || var.name == "apparentTemperature" || var.name == "dewPoint" || var.name == "temperatureMin" || var.name == "temperatureMax" ||
		var.name == "apparentTemperatureMax" || var.name == "apparentTemperatureMin") {
		variable = var.strVar + char(248) + "F";
	}
	else if (var.name == "windSpeed") {
		variable = var.strVar + " mph";
	}
	else if (var.name == "pressure") {
		variable = to_string(var.doubleVar * 0.000986923) + " atm";
	}
	else if (var.name == "ozone") {
		variable = var.strVar + " DU";
	}

	else if (var.name == "sunriseTime" || var.name == "sunsetTime") {
		time_t dataTime = (time_t)var.intVar;
		int offset = CONCERO::GetIntVariable("offset", weatherData);
		dataTime = dataTime + (offset * 3600);
		tm dataTM = *gmtime(&dataTime);
		variable = GetTime(dataTM, true, true);
	}
	else if (var.name == "temperatureMinTime" || var.name == "temperatureMaxTime" || var.name == "apparentTemperatureMinTime" || var.name == "apparentTemperatureMaxTime" || var.name == "precipIntensityMaxTime") {
		time_t dataTime = (time_t)var.intVar;
		int offset = CONCERO::GetIntVariable("offset", weatherData);
		dataTime = dataTime + (offset * 3600);
		tm dataTM = *gmtime(&dataTime);
		variable = GetTime(dataTM, true, false);
	}

	else if (var.name == "precipType" || var.name == "icon") {
		variable = ConvertVartName(var.strVar);
	}
	else {
		variable = var.strVar;
	}
	return(variable);
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
	bool update = true, data = false;
	if (updateData.data[1].stringVectorValue.size() > 0) {
		data = true;
	}
	SetWindowLayout();
	if (data == true) {
		LoadCurrentData();
	}
	while (running == true) {
		if (update == true) {
			update = false;
			if (data == true) {
				DrawData();
			}
			else if (data == false) {
				int tempX, tempY;
				CONSCIENTIA::FGetWindowSize("locationData", tempX, tempY);
				CONSCIENTIA::FMPrint("locationData", CONSCIENTIA::FindTextStart("Please Enter A Location", tempX), 1, "Please Enter A Location");
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
			CONSCIENTIA::Update();
		}
		input = CONSCIENTIA::Gint();
		if (input == 27) {
			string strInput = "";
			int tempX, tempY;
			CONSCIENTIA::GetWindowSize(0, tempX, tempY);
			strInput = CONSCIENTIA::Menu("Menu.lux", (tempX / 3), (tempY / 2) - 5, (tempX / 3), 10);
			if (strInput == "Quit") {
				running = false;
				LUXLECTOR::SaveDataFile("ProgramData.lux", updateData);
			}
			if (strInput == "Return") {
				update = true;
			}
			if (strInput == "New Location") {
				update = true;
				NewLocation();
				if (updateData.data[1].stringVectorValue.size() > 0) {
					data = true;
					LoadCurrentData();
				}
				else {
					data = false;
				}
			}
			if (strInput == "Delete Location") {
				RemoveLocation();
				update = true;
				if (updateData.data[1].stringVectorValue.size() > 0) {
					data = true;
				}
				else {
					data = false;
				}
				if (data == true) {
					LoadCurrentData();
				}
			}
		}
		if (input == 97 && day == true && data == true) {
			update = true;
			day = false;
			currentForcast = 0;
		}
		if (input == 100 && day == false && data == true) {
			update = true;
			day = true;
			currentForcast = 0;
		}
		if (input == 119 && currentForcast > 0 && data == true) {
			update = true;
			currentForcast--;
		}
		if (input == 115 && currentForcast < maxForcast && data == true) {
			update = true;
			currentForcast++;
		}
		if (input == 113 && currentLocation > 0 && data == true) {
			update = true;
			currentLocation--;
			currentForcast = 0;
			LoadCurrentData();
		}
		if (input == 101 && currentLocation < updateData.data[1].stringVectorValue.size() - 1 && data == true) {
			update = true;
			currentLocation++;
			currentForcast = 0;
			LoadCurrentData();
		}
		if (input == 114 && displayStartCurrent > 0 && data == true) {
			displayStartCurrent--;
			update = true;
		}
		if (input == 102 && data == true) {
			displayStartCurrent++;
			update = true;
		}
		if (input == 116 && displayStartForcast > 0 && data == true) {
			displayStartForcast--;
			update = true;
		}
		if (input == 103 && data == true) {
			displayStartForcast++;
			update = true;
		}
	}
}

void AER::NewLocation()
{
	int x, y;
	char inchar;
	int inint = 0;
	string line;
	CONSCIENTIA::GetWindowSize(0, x, y);
	CONSCIENTIA::GenorateWindow("New Location", (x / 3), (y / 2) - 2, (x / 3), 4, true, true);
	while (inint != 13) {
		CONSCIENTIA::FMPrint("New Location", CONSCIENTIA::FindTextStart(line, (x / 3)), 2, line);
		CONSCIENTIA::DrawBorder(CONSCIENTIA::FindWindowPointer("New Location"));
		CONSCIENTIA::DrawTitle(CONSCIENTIA::FindWindowPointer("New Location"));
		CONSCIENTIA::Update();
		inint = CONSCIENTIA::Gint();
		inchar = char(inint);
		if (inint != 13 && inint != 8) {
			line = line + inchar;
		}
		if (inint == 8 && line.size() > 0) {
			string temp = "";
			for (unsigned a = 0; a < line.size() - 1; a++) {
				temp = temp + line[a];
			}
			line = temp;
		}
	}
	CONSCIENTIA::TerminateWindow(CONSCIENTIA::FindWindowPointer("New Location"));
	string locationName = "";
	for (unsigned a = 0; a < line.size(); a++) {
		if (line[a] != ' ') {
			if (int(line[a]) > 91) {
				locationName = locationName + char(int(line[a]) - 32);
			}
			else {
				locationName = locationName + line[a];
			}
		}
	}
	updateData.data[3].stringVectorValue.push_back(locationName);
}

void AER::RemoveLocation()
{
	currentLocation = 0;
	currentForcast = 0;
	displayStartCurrent = 0;
	displayStartForcast = 0;
	int x, y;
	int inInt = -1;
	bool update = true;
	int selected = 0;
	CONSCIENTIA::GetWindowSize(0, x, y);
	CONSCIENTIA::GenorateWindow("Delete Location", x / 3, y / 4, x / 3, y / 2, true, true);
	while (inInt != 13 && inInt != 27) {
		if (update == true) {
			for (unsigned a = 0; a < updateData.data[1].stringVectorValue.size(); a++) {
				string line = updateData.data[1].stringVectorValue[a];
				if (a == selected) {
					line = ">" + line + "<";
				}
				CONSCIENTIA::FMPrint("Delete Location", CONSCIENTIA::FindTextStart(line, x / 3), a + 1, line);
			}
			CONSCIENTIA::DrawBorder(CONSCIENTIA::FindWindowPointer("Delete Location"));
			CONSCIENTIA::DrawTitle(CONSCIENTIA::FindWindowPointer("Delete Location"));
			CONSCIENTIA::Update();
			update = false;
		}
		inInt = CONSCIENTIA::Gint();
		if (inInt == 115 && selected < updateData.data[1].stringVectorValue.size() - 1) {
			selected++;
			update = true;
		}
		if (inInt == 119 && selected > 0) {
			selected--;
			update = true;
		}
	}
	if (inInt == 13 && selected < updateData.data[1].stringVectorValue.size()) {
		updateData.data[2].doubleVectorValue.erase(updateData.data[2].doubleVectorValue.begin() + (selected * 2) + 1);
		updateData.data[2].doubleVectorValue.erase(updateData.data[2].doubleVectorValue.begin() + (selected * 2));
		updateData.data[1].stringVectorValue.erase(updateData.data[1].stringVectorValue.begin() + selected);
	}
}