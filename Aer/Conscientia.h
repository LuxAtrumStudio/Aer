#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;
struct menuList {
	string name;
	vector<string> items;
};
struct menuPage {
	string name;
	vector<menuList> lists;
};
struct menuHierarchy {
	string name;
	vector<menuPage> pages;
};
namespace CONSCIENTIA {
	/*=====>>>>>-----CORE FUNCTIONS-----<<<<<=====*/
	/*=====>>>>>-----Initilization-----<<<<<=====*/
	void InitializeConscientia();
	void AdvancedInit(bool cursor, bool echo, bool raw);
	/*>>>>>-----SETTINGS-----<<<<<*/
	void SetAutoRefresh(bool setting);
	/*=====>>>>>-----Run Time-----<<<<<=====*/
	/*>>>>>-----WINDOW-----<<<<<*/
	/*>>>>>-----Initilization-----<<<<<*/
	void GenorateWindow(string name, int posX, int posY, int sizeX, int sizeY, bool border, bool title);
	/*>>>>>-----Management-----<<<<<*/
	void ClearAllWindows();
	int FindWindowPointer(string name);
	/*-----POINTER-----*/
	void SetBorder(int pointer, bool setting);
	void ClearWindow(int pointer);
	void SetWindowTitle(int pointer, bool setting);
	void SetCurrentWindow(int pointer);
	void GetWindowSize(int pointer, int& x, int& y);
	/*-----CURRENT-----*/
	void CSetBorder(bool setting);
	void CClearWindow();
	void CSetWindowTitle(bool setting);
	void CGetWindowSize(int& x, int& y);
	/*-----FIND-----*/
	void FSetBorder(string name, bool setting);
	void FClearWindow(string name);
	void FSetWindowTitle(string name, bool setting);
	void FSetCurrentWindow(string name);
	void FGetWindowSize(string name, int& x, int& y);
	/*-----WINDOWS-----*/
	void DrawBorder(int pointer);
	void DrawTitle(int pointer);
	/*>>>>>-----Termination-----<<<<<*/
	void TerminateAllWindows();
	void TerminateWindow(int pointer);
	/*>>>>>-----USER INTERFACE-----<<<<<*/
	/*>>>>>-----Input-----<<<<<*/
	char Gchar();
	int Gint();
	int Cint();
	string Cstr();
	float Gfloat();
	/*>>>>>-----Output-----<<<<<*/
	/*-----POINTER-----*/
	void Print(int pointer, string str);
	void MPrint(int pointer, int x, int y, string str);
	/*-----CURRENT-----*/
	void CPrint(string str);
	void CMPrint(int x, int y, string str);
	/*-----FIND-----*/
	void FPrint(string name, string str);
	void FMPrint(string name, int x, int y, string str);
	/*>>>>>-----SYSTEM-----<<<<<*/
	/*>>>>>-----Update-----<<<<<*/
	void Update();
	/*>>>>>-----Console-----<<<<<*/
	void SetConsoleName(string title);
	/*=====>>>>>-----Termination-----<<<<<=====*/
	void TerminateConscientia();
	/*=====>>>>>-----ADVANCED FUNCITONS-----<<<<<=====*/
	/*=====>>>>>-----Output Funcitons-----<<<<<=====*/
	/*>>>>>-----INTERACTIVE-----<<<<<*/
	/*>>>>>-----Menu-----<<<<<*/
	string Menu(string menuFileDirectory, int posX, int posY, int sizeX, int sizeY);
	menuHierarchy LoadMenuHierarchy(string menuFileDirectory);
	void DisplayMenu(menuHierarchy menu, int currentPage, int currentList, int currentItem);
	int FindTextStart(string str, int space);
	/*>>>>>-----DISPLAY-----<<<<<*/
	/*>>>>>-----Loading Bars-----<<<<<*/
	int InitializeLoadingBar(string process);
	void LoadingBar(int index, double percent);
	void TerminateLoadingBar(int index);
	/*=====>>>>>-----Input Funcitons-----<<<<<=====*/
	/*=====>>>>>-----System Funcitons-----<<<<<=====*/
	bool FullStartUp(bool border, bool title);
}

namespace LOGGING {
	void InitializeLogging();
	void LogError(string log, string location);
	void LogWarning(string log, string location);
	void LogSuccess(string log, string location);
	void LogData(string log, string location);
	void Log(int code);
	void TerminateLogging();
}

struct subVar {
	string name;
	vector<subVar> vars;
	string strVar;
	int intVar;
	double doubleVar;
};
struct JSONFile {
	vector<subVar> variables;
};
namespace CONCERO {
	void RunScript(vector<string> scriptData);
	void InterpretJSON(string file);
	subVar ReadVar(string name, string raw);
	JSONFile GetJSONFile(string name);
}