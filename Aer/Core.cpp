#include <iostream>
#include <ctime>
#include "Conscientia.h"
#include "Aer.h"
using namespace std;

int main() {
	cout << "Loading Weather Data\n";
	cout << "Depending on Internet Conection This May Take Up To Minute or more\n";
	cout << ">>Gathering Location Data\n";
	//AER::GetWOEID("RENO");
	cout << ">>Gathering Forcast Data\n";
	//AER::GetWeatherData(39.70866, -119.699341);
	cout << ">>Data Gathering Compleated\n>>Begining Program\n";
	time_t timea;
	ctime(&timea);
	cout << timea << endl;
	CONSCIENTIA::InitializeConscientia();
	CONSCIENTIA::Gchar();
	CONSCIENTIA::TerminateConscientia();
	return(1);
}