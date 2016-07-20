#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

namespace CONSCIENTIA {
	/*=====>>>>>-----CORE FUNCTIONS-----<<<<<=====*/
	/*=====>>>>>-----Initilization-----<<<<<=====*/
	void InitializeConscientia();
	void AdvancedInit(bool cursor, bool echo, bool raw);
	/*>>>>>-----SETTINGS-----<<<<<*/
	void SetAutoRefresh(bool setting);
	/*>>>>>-----NCURSES-----<<<<<*/
	void SetCursor(bool setting);
	void SetEcho(bool setting);
	void SetRaw(bool setting);
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
	void GetWindowSize(int pointer, int& x, int &y);
	/*-----CURRENT-----*/
	void CSetBorder(bool setting);
	void CClearWindow();
	void CSetWindowTitle(bool setting);
	void CGetWindowsize(int& x, int& y);
	/*-----FIND-----*/
	void FSetBorder(string name, bool setting);
	void FClearWindow(string name);
	void FSetWindowTitle(string name, bool setting);
	void FSetCurrentWindow(string name);
	void FGetWindowSize(string name, int& x, int &y);
	/*-----NCURSES-----*/
	void DrawBorder(int pointer, bool setting);
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
	float Cfloat();
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
	void SetConsoleName(stirng title);
	/*=====>>>>>-----Termination-----<<<<<=====*/
	void TerminateConscientia();
	/*=====>>>>>-----ADVANCED FUNCITONS-----<<<<<=====*/
	/*=====>>>>>-----Output Funcitons-----<<<<<=====*/
	/*>>>>>-----INTERACTIVE-----<<<<<*/
	/*>>>>>-----Menu-----<<<<<*/
	string Menu(string menuFileDirectory, int posX, int posY, int sizeX, int sizeY);
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
