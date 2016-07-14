#include <iostream>
#include <time.h>
#include <conio.h>
#include "Conscientia Files\Conscientia Headers.h"
#include "Aer.h"
using namespace std;

int main() {
	_getch();
	LOGGING::InitializeLogging();
	AER::ProgramStartUp();
	CONSCIENTIA::InitializeConscientia();
	AER::RunProgram();
	CONSCIENTIA::TerminateConscientia();
	LOGGING::TerminateLogging();
	return(1);
}