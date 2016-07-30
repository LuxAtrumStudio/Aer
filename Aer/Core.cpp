#include <iostream>
#include <time.h>
#include <conio.h>
#include "Conscientia Files\Conscientia Headers.h"
#include <SDL.h>
#undef main
#include "Aer.h"
using namespace std;

int main() {
	LOGGING::InitializeLogging();
	CONSCIENTIA::SetConsoleName("Aer Weather");
	AER::ProgramStartUp();
	CONSCIENTIA::InitializeConscientia();
	AER::RunProgram();
	CONSCIENTIA::TerminateConscientia();
	LOGGING::TerminateLogging();
	return(1);
}