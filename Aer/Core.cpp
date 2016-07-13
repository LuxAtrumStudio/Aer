#include <iostream>
#include <Python.h>
#include "Conscientia.h"
#include "Aer.h"
using namespace std;

int main() {
	cout << "Loading Weather Data\n";
	cout << "Depending on Internet Conection This May Take Up To Minute or more\n";
	cout << ">>Gathering Location Data\n";
	//AER::GetWOEID("RENO");
	cout << ">>Gathering Forcast Data\n";
	//AER::GetWeatherData(0);
	cout << ">>Data Gathering Compleated\n>>Begining Program\n\n\n";
	vector<string> scriptData = {
		"", "https://api.forecast.io/forecast/9c9c48f9ab5717ed899e8b6d730883c6/39.70866,-119.699341", "WeatherOut.json"
	};
	CONCERO::RunScript(scriptData);
	while (1);
	CONSCIENTIA::InitializeConscientia();
	CONSCIENTIA::Gchar();
	CONSCIENTIA::TerminateConscientia();
	return(1);
}