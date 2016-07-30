#include <iostream>
#include <time.h>
#include <conio.h>
#include "Conscientia Files\Conscientia Headers.h"
#include "Aequus Files\Aequus Headers.h"
#include "Aer.h"
using namespace std;

int main(int argc, char* argv[]) {
	LOGGING::InitializeLogging();
	AEQUUS::FullStatup();
	AER::ProgramStartUp();
	AER::RunProgram();
	AEQUUS::FullShutDown();
	LOGGING::TerminateLogging();
	return(1);
}