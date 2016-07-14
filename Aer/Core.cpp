#include <iostream>
#include <time.h>
#include <conio.h>
#include "Conscientia Files\Conscientia Headers.h"
#include "Aer.h"
using namespace std;

int main() {
	LOGGING::InitializeLogging();
	AER::ProgramStartUp();
	cout << "Loading Weather Data\n";
	cout << "Depending on Internet Conection This May Take Up To Minute or more\n";
	cout << ">>Gathering Location Data\n";
	//AER::GetWOEID("RENO");
	cout << ">>Gathering Forcast Data\n";
	//AER::GetWeatherData(39.70866, -119.699341);
	cout << ">>Data Gathering Compleated\n>>Begining Program\n";
	_getch();
	//CONSCIENTIA::InitializeConscientia();
	//CONSCIENTIA::Gchar();
	//CONSCIENTIA::TerminateConscientia();
	LOGGING::TerminateLogging();
	return(1);
}