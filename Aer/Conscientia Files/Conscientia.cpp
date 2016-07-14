#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <ctime>
#include <conio.h>
#include "Conscientia Headers.h"
/*=====>>>>>-----BASIC DATA-----<<<<<=====*/
/*=====>>>>>-----Global-----<<<<<=====*/
struct window {
	string name;
	int startX, startY, sizeX, sizeY, endX, endY;
	int cursorX, cursorY;
	bool border, title;
};
vector<window> windows;
int currentWindowPointer = 0;
bool autoRefresh = false;
/*=====>>>>>-----Windows-----<<<<<=====*/
int currentBuffer = 1;
HANDLE loadBuffer, displayBuffer;
_CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

/*=====>>>>>-----ADVANCED DATA-----<<<<<=====*/
/*=====>>>>>-----Global-----<<<<<=====*/
/*>>>>>-----FUNCTIONS-----<<<<<*/
/*>>>>>-----Menu-----<<<<<*/
int firstPage = 0, firstList = 0, firstItem = 0;
/*>>>>>-----Loading Bars-----<<<<<*/
vector<int> loadingBarPointers;

/*=====>>>>>-----FUNCTIONS-----<<<<<=====*/
/*=====>>>>>-----Initilization-----<<<<<=====*/
void CONSCIENTIA::InitializeConscientia() {
	autoRefresh = false;
	loadBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	displayBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(displayBuffer);
	GetConsoleScreenBufferInfo(displayBuffer, &consoleInfo);
	window declaration;
	declaration.name = "stdscr";
	declaration.startX = 0;
	declaration.startY = 0;
	declaration.sizeX = consoleInfo.dwSize.X;
	declaration.sizeY = consoleInfo.dwSize.Y;
	declaration.endX = consoleInfo.dwSize.X;
	declaration.endY = consoleInfo.dwSize.Y;
	declaration.cursorX = 0;
	declaration.cursorY = 0;
	declaration.border = false;
	declaration.title = false;
	windows.push_back(declaration);
}
void CONSCIENTIA::AdvancedInit(bool cursor, bool echo, bool raw) {
	InitializeConscientia();
}
/*>>>>>-----SETTINGS-----<<<<<*/
void CONSCIENTIA::SetAutoRefresh(bool setting) {
	autoRefresh = setting;
}
/*=====>>>>>-----Run Time-----<<<<<=====*/
/*>>>>>-----WINDOW-----<<<<<*/
/*>>>>>-----Initilization-----<<<<<*/
void CONSCIENTIA::GenorateWindow(string name, int posX, int posY, int sizeX, int sizeY, bool border, bool title) {
	window declaration;
	declaration.name = name;
	declaration.startX = posX;
	declaration.startY = posY;
	declaration.sizeX = sizeX;
	declaration.sizeY = sizeY;
	declaration.endX = posX + sizeX;
	declaration.endY = posY + sizeY;
	declaration.cursorX = 0;
	declaration.cursorY = 0;
	declaration.border = border;
	declaration.title = title;
	if (border == true) {
		declaration.cursorX++;
		declaration.cursorY++;
	}
	if (title == true && border == false) {
		declaration.cursorY++;
	}
	windows.push_back(declaration);
	if (border == true) {
		DrawBorder(windows.size() - 1);
	}
	if (title == true) {
		DrawTitle(windows.size() - 1);
	}
}
/*>>>>>-----Management-----<<<<<*/
void CONSCIENTIA::ClearAllWindows() {
	for (unsigned a = 0; a < windows.size(); a++) {
		ClearWindow(a);
	}
}
int CONSCIENTIA::FindWindowPointer(string name) {
	for (unsigned a = 0; a < windows.size(); a++) {
		if (windows[a].name == name) {
			return(a);
		}
	}
	return(0);
}
/*-----POINTER-----*/
void CONSCIENTIA::SetBorder(int pointer, bool setting) {
	if (windows[pointer].border != setting) {
		windows[pointer].border = setting;
		DrawBorder(pointer);
	}
}
void CONSCIENTIA::ClearWindow(int pointer) {
	string clear = "";
	int charCount, x, y;
	windows[pointer].cursorX = 0;
	windows[pointer].cursorY = 0;
	if (windows[pointer].border == true || windows[pointer].title == true) {
		windows[pointer].cursorY++;
	}
	if (windows[pointer].border == true) {
		windows[pointer].cursorX++;
	}
	x = windows[pointer].sizeX;
	y = windows[pointer].sizeY;
	if (windows[pointer].border == true) {
		x -= 2;
		y -= 2;
	}
	if (windows[pointer].title == true && windows[pointer].border == false) {
		x--;
	}
	charCount = x;
	for (int a = 0; a < charCount; a++) {
		clear = clear + char(0);
	}
	for (int a = 0; a < y; a++) {
		Print(pointer, clear);
	}
	clear = "";
	windows[pointer].cursorX = 0;
	windows[pointer].cursorY = 0;
	if (windows[pointer].border == true || windows[pointer].title == true) {
		windows[pointer].cursorY++;
	}
	if (windows[pointer].border == true) {
		windows[pointer].cursorX++;
	}
}
void CONSCIENTIA::SetWindowTitle(int pointer, bool setting) {
	if (windows[pointer].title != setting) {
		windows[pointer].title = setting;
		DrawTitle(pointer);
	}
}
void CONSCIENTIA::SetCurrentWindow(int pointer) {
	currentWindowPointer = pointer;
}
void CONSCIENTIA::GetWindowSize(int pointer, int& x, int& y) {
	x = windows[pointer].sizeX;
	y = windows[pointer].sizeY;
}
/*-----CURRENT-----*/
void CONSCIENTIA::CSetBorder(bool setting) {
	SetBorder(currentWindowPointer, setting);
}
void CONSCIENTIA::CClearWindow() {
	ClearWindow(currentWindowPointer);
}
void CONSCIENTIA::CSetWindowTitle(bool setting) {
	SetWindowTitle(currentWindowPointer, setting);
}
void CONSCIENTIA::CGetWindowSize(int& x, int& y) {
	x = windows[currentWindowPointer].sizeX;
	y = windows[currentWindowPointer].sizeY;
}
/*-----FIND-----*/
void CONSCIENTIA::FSetBorder(string name, bool setting) {
	SetBorder(FindWindowPointer(name), setting);
}
void CONSCIENTIA::FClearWindow(string name) {
	ClearWindow(FindWindowPointer(name));
}
void CONSCIENTIA::FSetWindowTitle(string name, bool setting) {
	SetWindowTitle(FindWindowPointer(name), setting);
}
void CONSCIENTIA::FSetCurrentWindow(string name) {
	currentWindowPointer = FindWindowPointer(name);
}
void CONSCIENTIA::FGetWindowSize(string name, int& x, int& y) {
	x = windows[FindWindowPointer(name)].sizeX;
	y = windows[FindWindowPointer(name)].sizeY;
}
/*-----WINDOWS-----*/
void CONSCIENTIA::DrawBorder(int pointer) {
	int x = 0, y = 0;
	char TL = char(218), TR = char(191), BL = char(192), BR = char(217);
	char V = char(179), H = char(196);
	string characterH;
	stringstream sstreamH;
	COORD posH;
	DWORD dwBytesWrittenH = 0;
	sstreamH << H;
	sstreamH >> characterH;
	for (x = 0; x <= windows[pointer].sizeX - 1; x++) {
		posH = { (short)x + (short)windows[pointer].startX, (short)windows[pointer].startY };
		WriteConsoleOutputCharacter(loadBuffer, characterH.c_str(), characterH.size(), posH, &dwBytesWrittenH);
		posH = { (short)x + (short)windows[pointer].startX, (short)windows[pointer].endY - 1 };
		WriteConsoleOutputCharacter(loadBuffer, characterH.c_str(), characterH.size(), posH, &dwBytesWrittenH);
	}
	string characterV;
	stringstream sstreamV;
	COORD posV;
	DWORD dwBytesWrittenV = 0;
	sstreamV << V;
	sstreamV >> characterV;
	for (y = 0; y <= windows[pointer].sizeY - 1; y++) {
		posV = { (short)windows[pointer].startX, (short)y + (short)windows[pointer].startY };
		WriteConsoleOutputCharacter(loadBuffer, characterV.c_str(), characterV.size(), posV, &dwBytesWrittenV);
		posV = { (short)windows[pointer].endX - 1, (short)y + (short)windows[pointer].startY };
		WriteConsoleOutputCharacter(loadBuffer, characterV.c_str(), characterV.size(), posV, &dwBytesWrittenV);
	}
	string character1;
	stringstream sstream1;
	COORD pos1;
	DWORD dwBytesWritten1 = 0;
	sstream1 << TL;
	sstream1 >> character1;
	pos1 = { (short)windows[pointer].startX, (short)windows[pointer].startY };
	WriteConsoleOutputCharacter(loadBuffer, character1.c_str(), character1.size(), pos1, &dwBytesWritten1);
	string character2;
	stringstream sstream2;
	COORD pos2;
	DWORD dwBytesWritten2 = 0;
	sstream2 << TR;
	sstream2 >> character2;
	pos2 = { (short)windows[pointer].endX - 1, (short)windows[pointer].startY };
	WriteConsoleOutputCharacter(loadBuffer, character2.c_str(), character2.size(), pos2, &dwBytesWritten2);
	string character3;
	stringstream sstream3;
	COORD pos3;
	DWORD dwBytesWritten3 = 0;
	sstream3 << BL;
	sstream3 >> character3;
	pos3 = { (short)windows[pointer].startX, (short)windows[pointer].endY - 1 };
	WriteConsoleOutputCharacter(loadBuffer, character3.c_str(), character3.size(), pos3, &dwBytesWritten3);
	string character4;
	stringstream sstream4;
	COORD pos4;
	DWORD dwBytesWritten4 = 0;
	sstream4 << BR;
	sstream4 >> character4;
	pos4 = { (short)windows[pointer].endX - 1, (short)windows[pointer].endY - 1 };
	WriteConsoleOutputCharacter(loadBuffer, character4.c_str(), character4.size(), pos4, &dwBytesWritten4);
}
void CONSCIENTIA::DrawTitle(int pointer) {
	int titleSize = windows[pointer].name.size();
	int windowSize = windows[pointer].sizeX;
	windowSize = windowSize / 2;
	titleSize = titleSize / 2;
	COORD pos = { (windowSize - titleSize) + windows[pointer].startX, windows[pointer].startY };
	DWORD dwBytesWritten;
	string title = windows[pointer].name;
	WriteConsoleOutputCharacter(loadBuffer, title.c_str(), title.size(), pos, &dwBytesWritten);
}
/*>>>>>-----Termination-----<<<<<*/
void CONSCIENTIA::TerminateAllWindows() {
	for (unsigned a = 1; a < windows.size(); a++) {
		TerminateWindow(a);
	}
}
void CONSCIENTIA::TerminateWindow(int pointer) {
	if (windows[pointer].border == true) {
		SetBorder(pointer, false);
	}
	if (windows[pointer].title == true) {
		SetWindowTitle(pointer, false);
	}
	ClearWindow(pointer);
	windows.erase(windows.begin() + pointer);
}
/*>>>>>-----USER INTERFACE-----<<<<<*/
/*>>>>>-----Input-----<<<<<*/
char CONSCIENTIA::Gchar() {
	char in;
	in = _getch();
	return(in);
}
int CONSCIENTIA::Gint() {
	int in;
	in = _getch();
	return(in);
}
int CONSCIENTIA::Cint() {
	int in = 0;
	int rawin = 0;
	while (rawin != 13) {
		rawin = _getch();
		in = (in * 10) + rawin;
	}
	return(in);
}
string CONSCIENTIA::Cstr() {
	string in;
	char inch;
	int rawint = 0;
	while (rawint != 13) {
		rawint = _getch();
		inch = char(rawint);
		in = in + inch;
	}
	return(in);
}
float CONSCIENTIA::Gfloat() {
	string rawstr = Cstr();
	string::size_type sz;
	float in;
	in = stof(rawstr, &sz);
	return(in);
}
/*>>>>>-----Output-----<<<<<*/
/*-----POINTER-----*/
void CONSCIENTIA::Print(int pointer, string str) {
	for (int a = 0; a < str.size(); a++) {
		if (str[a] == '/') {
			int b = a + 1;
			if (str[b] == 'n') {
				windows[pointer].cursorY++;
				if (windows[pointer].cursorY > windows[pointer].sizeY) {
					windows[pointer].cursorY = 0;
					if (windows[pointer].border == true || windows[pointer].title == true) {
						windows[pointer].cursorY++;
					}
				}
				ClearWindow(pointer);
				a = a + 2;
			}
		}
		else if (str[a] != '/') {
			if (windows[pointer].border == false) {
				if (windows[pointer].cursorX >= windows[pointer].sizeX) {
					windows[pointer].cursorX = 0;
					windows[pointer].cursorY++;
					if (windows[pointer].cursorY >= windows[pointer].sizeY) {
						windows[pointer].cursorY = 0;
						if (windows[pointer].title == true) {
							windows[pointer].cursorY++;
						}
						ClearWindow(pointer);
					}
				}
			}
			if (windows[pointer].border == true) {
				if (windows[pointer].cursorX >= windows[pointer].sizeX - 1) {
					windows[pointer].cursorX = 1;
					windows[pointer].cursorY++;
					if (windows[pointer].cursorY >= windows[pointer].sizeY - 1) {
						windows[pointer].cursorY = 1;
						ClearWindow(pointer);
					}
				}
			}
			string character;
			stringstream sstream;
			sstream << str[a];
			sstream >> character;
			COORD pos = { windows[pointer].cursorX + windows[pointer].startX, windows[pointer].cursorY + windows[pointer].startY };
			DWORD dwBytesWritten = 0;
			WriteConsoleOutputCharacter(loadBuffer, character.c_str(), character.size(), pos, &dwBytesWritten);
			windows[pointer].cursorX++;
		}
	}
	if (autoRefresh == true) {
		Update();
	}
}
void CONSCIENTIA::MPrint(int pointer, int x, int y, string str) {
	windows[pointer].cursorX = x;
	windows[pointer].cursorY = y;
	Print(pointer, str);
}
/*-----CURRENT-----*/
void CONSCIENTIA::CPrint(string str) {
	Print(currentWindowPointer, str);
}
void CONSCIENTIA::CMPrint(int x, int y, string str) {
	MPrint(currentWindowPointer, x, y, str);
}
/*-----FIND-----*/
void CONSCIENTIA::FPrint(string name, string str) {
	Print(FindWindowPointer(name), str);
}
void CONSCIENTIA::FMPrint(string name, int x, int y, string str) {
	MPrint(FindWindowPointer(name), x, y, str);
}
/*>>>>>-----SYSTEM-----<<<<<*/
/*>>>>>-----Update-----<<<<<*/
void CONSCIENTIA::Update() {
	displayBuffer = loadBuffer;
	SetConsoleActiveScreenBuffer(displayBuffer);
	loadBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
}
/*>>>>>-----Console-----<<<<<*/
void CONSCIENTIA::SetConsoleName(string title) {
	SetConsoleTitle(title.c_str());
}
/*=====>>>>>-----Termination-----<<<<<=====*/
void CONSCIENTIA::TerminateConscientia() {
}

/*=====>>>>>-----ADVANCED FUNCITONS-----<<<<<=====*/
/*=====>>>>>-----Output Funcitons-----<<<<<=====*/
/*>>>>>-----INTERACTIVE-----<<<<<*/
/*>>>>>-----Menu-----<<<<<*/
string CONSCIENTIA::Menu(string menuFileDirectory, int posX, int posY, int sizeX, int sizeY) {
	bool run = true, update = true;
	int in = -1;
	menuHierarchy menuStruct;
	menuStruct = LUXLECTOR::LoadHierarchyFile(menuFileDirectory);
	int windowPointer = windows.size();
	int pageWidth, listWidth;
	int currentPage = 0, currentList = 0, currentItem = 0;
	GenorateWindow(menuStruct.name, posX, posY, sizeX, sizeY, true, true);
	SetCurrentWindow(windowPointer);
	while (run == true) {
		if (update == true) {
			update = false;
			DrawBorder(currentWindowPointer);
			DrawTitle(currentWindowPointer);
			DisplayMenu(menuStruct, currentPage, currentList, currentItem);
		}
		in = Gint();
		if (in == 'a' && currentList > 0) {
			currentList--;
			update = true;
			if (menuStruct.pages[currentPage].lists[currentList].items.size() <= currentItem) {
				currentItem = menuStruct.pages[currentPage].lists[currentList].items.size() - 1;
			}
		}
		if (in == 's' && currentItem < menuStruct.pages[currentPage].lists[currentList].items.size() - 1) {
			currentItem++;
			update = true;
		}
		if (in == 'd' && currentList < menuStruct.pages[currentPage].lists.size() - 1) {
			currentList++;
			update = true;
			if (menuStruct.pages[currentPage].lists[currentList].items.size() <= currentItem) {
				currentItem = menuStruct.pages[currentPage].lists[currentList].items.size() - 1;
			}
		}
		if (in == 'q' && currentPage > 0) {
			currentPage--;
			update = true;
			if (menuStruct.pages[currentPage].lists.size() <= currentList) {
				currentList = menuStruct.pages[currentPage].lists.size() - 1;
			}
			if (menuStruct.pages[currentPage].lists[currentList].items.size() <= currentItem) {
				currentItem = menuStruct.pages[currentPage].lists[currentList].items.size() - 1;
			}
		}
		if (in == 'w' && currentItem > 0) {
			currentItem--;
			update = true;
		}
		if (in == 'e' && currentPage < menuStruct.pages.size() - 1) {
			currentPage++;
			update = true;
			if (menuStruct.pages[currentPage].lists.size() <= currentList) {
				currentList = menuStruct.pages[currentPage].lists.size() - 1;
			}
			if (menuStruct.pages[currentPage].lists[currentList].items.size() <= currentItem) {
				currentItem = menuStruct.pages[currentPage].lists[currentList].items.size() - 1;
			}
		}
		if (in == 27) {
			firstItem = 0;
			firstList = 0;
			firstPage = 0;
			return("");
		}
		if (in == 13) {
			TerminateWindow(currentWindowPointer);
			return(menuStruct.pages[currentPage].lists[currentList].items[currentItem]);
		}
	}
	return("");
}

void CONSCIENTIA::DisplayMenu(menuHierarchy menu, int currentPage, int currentList, int currentItem) {
	CClearWindow();
	string v, h, i, line;
	char vc, hc, ic;
	stringstream vss, hss, iss;
	vc = char(179);
	hc = char(196);
	ic = char(193);
	vss << vc;
	vss >> v;
	hss << hc;
	hss >> h;
	iss << ic;
	iss >> i;
	int sizeX, sizeY;
	sizeX = windows[currentWindowPointer].sizeX - 2;
	sizeY = windows[currentWindowPointer].sizeY - 2;
	int pageWidth, listWidth;
	int pagesDisplayed = menu.pages.size(), listsDisplayed = menu.pages[currentPage].lists.size(), maxItemsDisplayed = windows[currentWindowPointer].sizeY - 5;
	int x, y, yn;
	pageWidth = sizeX / menu.pages.size();
	if (pageWidth < (sizeX - 4) / 5) {
		pageWidth = (sizeX - 3) / 4;
		pagesDisplayed = 4;
	}
	for (int a = 0; a < pageWidth - 1; a++) {
		line = line + h;
	}
	listWidth = sizeX / menu.pages[currentPage].lists.size();
	if (listWidth < sizeX / 9) {
		listWidth = sizeX / 8;
		listsDisplayed = 8;
	}

	while (currentPage >= pagesDisplayed + firstPage - 1 && pagesDisplayed + firstPage < menu.pages.size()) {
		firstPage++;
	}
	while (currentPage <= firstPage && firstPage > 0) {
		firstPage--;
	}
	while (currentList >= listsDisplayed + firstList - 1 && listsDisplayed + firstList < menu.pages[currentPage].lists.size()) {
		firstList++;
	}
	while (currentList <= firstList && firstList > 0) {
		firstList--;
	}
	while (currentItem >= maxItemsDisplayed + firstItem - 1 && maxItemsDisplayed + firstItem < menu.pages[currentPage].lists[currentList].items.size()) {
		firstItem++;
	}
	while (currentItem <= firstItem && firstItem > 0) {
		firstItem--;
	}

	x = 1;
	y = 1;
	for (int a = firstPage; a < pagesDisplayed + firstPage; a++) {
		CMPrint(x + FindTextStart(menu.pages[a].name, pageWidth), y, menu.pages[a].name);
		if (currentPage != a) {
			CMPrint(x + 1, y + 1, line);
		}
		x = x + pageWidth;
		if (a != pagesDisplayed - 1) {
			CMPrint(x, y, v);
			CMPrint(x, y + 1, i);
		}
	}
	x = 0;
	y = y + 2;
	for (int a = firstList; a < listsDisplayed + firstList; a++) {
		if (menu.pages[currentPage].lists[a].name != "") {
			CMPrint(x + FindTextStart("<" + menu.pages[currentPage].lists[a].name + ">", listWidth), y, "<" + menu.pages[currentPage].lists[a].name + ">");
		}
		yn = y + 1;
		for (int b = firstItem; b < maxItemsDisplayed + firstItem && b < menu.pages[currentPage].lists[a].items.size(); b++) {
			if (a == currentList && b == currentItem) {
				CMPrint(x + FindTextStart(">" + menu.pages[currentPage].lists[a].items[b] + "<", listWidth), yn, ">" + menu.pages[currentPage].lists[a].items[b] + "<");
			}
			else {
				CMPrint(x + FindTextStart(menu.pages[currentPage].lists[a].items[b], listWidth), yn, menu.pages[currentPage].lists[a].items[b]);
			}
			yn++;
		}
		x = x + listWidth;
	}
	Update();
}
int CONSCIENTIA::FindTextStart(string str, int space) {
	space = space / 2;
	int strSize = str.size();
	strSize = strSize / 2;
	int strStart = space - strSize;
	return(strStart);
}
/*>>>>>-----DISPLAY-----<<<<<*/
/*>>>>>-----Loading Bars-----<<<<<*/
int CONSCIENTIA::InitializeLoadingBar(string process) {
	loadingBarPointers.push_back(windows.size());
	GenorateWindow(process, (windows[0].sizeX / 2) - (windows[0].sizeX / 4), (windows[0].sizeY / 2) - 1, (windows[0].sizeX / 2), 3, true, true);
	return(loadingBarPointers.size() - 1);
}
void CONSCIENTIA::LoadingBar(int index, double percent) {
	char block = char(219);
	int loadingBarPointer = loadingBarPointers[index];
	int size = windows[loadingBarPointer].sizeX - 2;
	double blockWorth = (double)100 / (double)size;
	ClearWindow(loadingBarPointer);
	string bar = "";
	while (percent > blockWorth) {
		bar = bar + block;
		percent = percent - blockWorth;
	}
	Print(loadingBarPointer, bar);
}
void CONSCIENTIA::TerminateLoadingBar(int index) {
	TerminateWindow(loadingBarPointers[index]);
	loadingBarPointers.erase(loadingBarPointers.begin() + index);
}
/*=====>>>>>-----Input Funcitons-----<<<<<=====*/
/*=====>>>>>-----System Funcitons-----<<<<<=====*/
bool CONSCIENTIA::FullStartUp(bool border, bool title) {
	InitializeConscientia();
	LOGGING::InitializeLogging();
	GenorateWindow("Main", 0, 0, windows[0].sizeX - 2, windows[0].sizeY - 2, border, title);
	return(true);
}