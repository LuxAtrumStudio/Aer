#include <iostream>
#include <time.h>
#include <conio.h>
#include "Conscientia Files\Conscientia Headers.h"
#include "Aer.h"
using namespace std;

int main() {
	LOGGING::InitializeLogging();
	AER::ProgramStartUp();
	//CONSCIENTIA::InitializeConscientia();
	//CONSCIENTIA::Gchar();
	//CONSCIENTIA::TerminateConscientia();
	LOGGING::TerminateLogging();
	return(1);
}