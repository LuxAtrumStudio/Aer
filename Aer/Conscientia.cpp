#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <ctime>
#include <conio.h>
#include "Conscientia.h"
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
bool initConscientia = false;
/*=====>>>>>-----Windows-----<<<<<=====*/
int currentBuffer = 1;
HANDLE loadBuffer, displayBuffer;
_CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

/*=====>>>>>-----ADVANCED DATA-----<<<<<=====*/
/*=====>>>>>-----Global-----<<<<<=====*/
struct luxCode {
	vector<string> lines;
};
/*>>>>>-----FUNCTIONS-----<<<<<*/
/*>>>>>-----Menu-----<<<<<*/
int firstPage = 0, firstList = 0, firstItem = 0;
/*>>>>>-----Loading Bars-----<<<<<*/
vector<int> loadingBarPointers;

namespace CONSCIENTIA {
	/*=====>>>>>-----FUNCTIONS-----<<<<<=====*/
	/*=====>>>>>-----Initilization-----<<<<<=====*/
	void InitializeConscientia() {
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
		initConscientia = true;
	}
	void AdvancedInit(bool cursor, bool echo, bool raw) {
		InitializeConscientia();
	}
	/*>>>>>-----SETTINGS-----<<<<<*/
	void SetAutoRefresh(bool setting) {
		autoRefresh = setting;
	}
	/*=====>>>>>-----Run Time-----<<<<<=====*/
	/*>>>>>-----WINDOW-----<<<<<*/
	/*>>>>>-----Initilization-----<<<<<*/
	void GenorateWindow(string name, int posX, int posY, int sizeX, int sizeY, bool border, bool title) {
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
	void ClearAllWindows() {
		for (unsigned a = 0; a < windows.size(); a++) {
			ClearWindow(a);
		}
	}
	int FindWindowPointer(string name) {
		for (unsigned a = 0; a < windows.size(); a++) {
			if (windows[a].name == name) {
				return(a);
			}
		}
		return(0);
	}
	/*-----POINTER-----*/
	void SetBorder(int pointer, bool setting) {
		if (windows[pointer].border != setting) {
			windows[pointer].border = setting;
			DrawBorder(pointer);
		}
	}
	void ClearWindow(int pointer) {
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
	void SetWindowTitle(int pointer, bool setting) {
		if (windows[pointer].title != setting) {
			windows[pointer].title = setting;
			DrawTitle(pointer);
		}
	}
	void SetCurrentWindow(int pointer) {
		currentWindowPointer = pointer;
	}
	void GetWindowSize(int pointer, int& x, int& y) {
		x = windows[pointer].sizeX;
		y = windows[pointer].sizeY;
	}
	/*-----CURRENT-----*/
	void CSetBorder(bool setting) {
		SetBorder(currentWindowPointer, setting);
	}
	void CClearWindow() {
		ClearWindow(currentWindowPointer);
	}
	void CSetWindowTitle(bool setting) {
		SetWindowTitle(currentWindowPointer, setting);
	}
	void CGetWindowSize(int& x, int& y) {
		x = windows[currentWindowPointer].sizeX;
		y = windows[currentWindowPointer].sizeY;
	}
	/*-----FIND-----*/
	void FSetBorder(string name, bool setting) {
		SetBorder(FindWindowPointer(name), setting);
	}
	void FClearWindow(string name) {
		ClearWindow(FindWindowPointer(name));
	}
	void FSetWindowTitle(string name, bool setting) {
		SetWindowTitle(FindWindowPointer(name), setting);
	}
	void FSetCurrentWindow(string name) {
		currentWindowPointer = FindWindowPointer(name);
	}
	void FGetWindowSize(string name, int& x, int& y) {
		x = windows[FindWindowPointer(name)].sizeX;
		y = windows[FindWindowPointer(name)].sizeY;
	}
	/*-----WINDOWS-----*/
	void DrawBorder(int pointer) {
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
	void DrawTitle(int pointer) {
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
	void TerminateAllWindows() {
		for (unsigned a = 1; a < windows.size(); a++) {
			TerminateWindow(a);
		}
	}
	void TerminateWindow(int pointer) {
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
	char Gchar() {
		char in;
		in = _getch();
		return(in);
	}
	int Gint() {
		int in;
		in = _getch();
		return(in);
	}
	int Cint() {
		int in = 0;
		int rawin = 0;
		while (rawin != 13) {
			rawin = _getch();
			in = (in * 10) + rawin;
		}
		return(in);
	}
	string Cstr() {
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
	float Gfloat() {
		string rawstr = Cstr();
		string::size_type sz;
		float in;
		in = stof(rawstr, &sz);
		return(in);
	}
	/*>>>>>-----Output-----<<<<<*/
	/*-----POINTER-----*/
	void Print(int pointer, string str) {
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
	void MPrint(int pointer, int x, int y, string str) {
		windows[pointer].cursorX = x;
		windows[pointer].cursorY = y;
		Print(pointer, str);
	}
	/*-----CURRENT-----*/
	void CPrint(string str) {
		Print(currentWindowPointer, str);
	}
	void CMPrint(int x, int y, string str) {
		MPrint(currentWindowPointer, x, y, str);
	}
	/*-----FIND-----*/
	void FPrint(string name, string str) {
		Print(FindWindowPointer(name), str);
	}
	void FMPrint(string name, int x, int y, string str) {
		MPrint(FindWindowPointer(name), x, y, str);
	}
	/*>>>>>-----SYSTEM-----<<<<<*/
	/*>>>>>-----Update-----<<<<<*/
	void Update() {
		displayBuffer = loadBuffer;
		SetConsoleActiveScreenBuffer(displayBuffer);
		loadBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	}
	/*>>>>>-----Console-----<<<<<*/
	void SetConsoleName(string title) {
		SetConsoleTitle(title.c_str());
	}
	/*=====>>>>>-----Termination-----<<<<<=====*/
	void TerminateConscientia() {
	}

	/*=====>>>>>-----ADVANCED FUNCITONS-----<<<<<=====*/
	/*=====>>>>>-----Output Funcitons-----<<<<<=====*/
	/*>>>>>-----INTERACTIVE-----<<<<<*/
	/*>>>>>-----Menu-----<<<<<*/
	string Menu(string menuFileDirectory, int posX, int posY, int sizeX, int sizeY) {
		bool run = true, update = true;
		int in = -1;
		menuHierarchy menuStruct;
		menuStruct = LoadMenuHierarchy(menuFileDirectory);
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
	menuHierarchy LoadMenuHierarchy(string menuFileDirectory) {
		string line;
		luxCode rawCode;
		int lineCount = 0, totalLines;
		int loadBar = -1;
		ifstream load(menuFileDirectory.c_str());
		if (load.is_open()) {
			load >> lineCount;
			totalLines = lineCount;
			if (lineCount >= 50) {
				loadBar = InitializeLoadingBar("Loading Menu");
			}
			lineCount = 0;
			getline(load, line);
			while (getline(load, line)) {
				rawCode.lines.push_back(line);
				lineCount++;
				if (loadBar != -1 && (lineCount % 10) == 0) {
					LoadingBar(loadBar, ((double)lineCount / (double)totalLines) * 100);
				}
			}
			load.close();
			if (loadBar != -1) {
				TerminateLoadingBar(loadBar);
			}
		}
		string cleanLine;
		bool tabSpace;
		for (unsigned a = 0; a < rawCode.lines.size(); a++) {
			line = rawCode.lines[a];
			cleanLine = "";
			tabSpace = true;
			for (unsigned b = 0; b < line.size(); b++) {
				if (tabSpace == true && line[b] != '	') {
					tabSpace = false;
				}
				if (tabSpace == false) {
					cleanLine = cleanLine + line[b];
				}
			}
			rawCode.lines[a] = cleanLine;
		}
		menuHierarchy newHierarchy;
		menuPage newPage;
		menuList newList;
		int currentLevel = 0;
		string currentLine = "";
		string codeLine = "";
		for (unsigned a = 0; a < rawCode.lines.size(); a++) {
			currentLine = rawCode.lines[a];
			codeLine = "";
			if (currentLine[0] == '[') {
				for (unsigned b = 1; b < currentLine.size() && currentLine[b] != ']'; b++) {
					codeLine = codeLine + currentLine[b];
				}
				if (currentLevel == 0) {
				}
				if (currentLevel == 1) {
				}
				if (currentLevel == 2) {
				}
			}
			if (currentLine[currentLine.size() - 1] == '{') {
				if (currentLevel == 0) {
					newHierarchy.name = codeLine;
				}
				if (currentLevel == 1) {
					newPage.lists.clear();
					newPage.name = codeLine;
				}
				if (currentLevel == 2) {
					newList.items.clear();
					newList.name = codeLine;
				}
				currentLevel++;
			}
			if (currentLine[currentLine.size() - 1] == '}') {
				currentLevel--;
				if (currentLevel == 0) {
				}
				if (currentLevel == 1) {
					newHierarchy.pages.push_back(newPage);
				}
				if (currentLevel == 2) {
					newPage.lists.push_back(newList);
				}
			}
			if (currentLine[0] != '[') {
				newList.items.push_back(currentLine);
			}
		}
		return(newHierarchy);
	}
	void DisplayMenu(menuHierarchy menu, int currentPage, int currentList, int currentItem) {
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
	int FindTextStart(string str, int space) {
		space = space / 2;
		int strSize = str.size();
		strSize = strSize / 2;
		int strStart = space - strSize;
		return(strStart);
	}
	/*>>>>>-----DISPLAY-----<<<<<*/
	/*>>>>>-----Loading Bars-----<<<<<*/
	int InitializeLoadingBar(string process) {
		loadingBarPointers.push_back(windows.size());
		GenorateWindow(process, (windows[0].sizeX / 2) - (windows[0].sizeX / 4), (windows[0].sizeY / 2) - 1, (windows[0].sizeX / 2), 3, true, true);
		return(loadingBarPointers.size() - 1);
	}
	void LoadingBar(int index, double percent) {
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
	void TerminateLoadingBar(int index) {
		TerminateWindow(loadingBarPointers[index]);
		loadingBarPointers.erase(loadingBarPointers.begin() + index);
	}
	/*=====>>>>>-----Input Funcitons-----<<<<<=====*/
	/*=====>>>>>-----System Funcitons-----<<<<<=====*/
	bool FullStartUp(bool border, bool title) {
		InitializeConscientia();
		LOGGING::InitializeLogging();
		GenorateWindow("Main", 0, 0, windows[0].sizeX - 2, windows[0].sizeY - 2, border, title);
		return(true);
	}
}

ofstream logFile;
bool logOpen = false;
bool initLogging = false;
namespace LOGGING {
	void InitializeLogging() {
		logFile.open("Log.log");
		if (logFile.is_open()) {
			logOpen = true;
		}
		time_t currentTime;
		string dateTime;
		currentTime = time(0);
		dateTime = ctime(&currentTime);
		string temp = "";
		for (int a = 0; a < dateTime.size() - 1; a++) {
			temp = temp + dateTime[a];
		}
		dateTime = temp;
		string File = "Log Data.log";
		if (logOpen == true) {
			LogSuccess("Created New Log File", "Logging.cpp/initializeLogging");
			LogData(dateTime, "Consolas.cpp/initializeLogging");
		}
		initLogging = true;
	}
	void LogError(string log, string location) {
		if (logOpen == true) {
			logFile << "[Error]:" + log + "[" + location + "]\n";
		}
	}
	void LogWarning(string log, string location) {
		if (logOpen == true) {
			logFile << "[Warning]:" + log + "[" + location + "]\n";
		}
	}
	void LogSuccess(string log, string location) {
		if (logOpen == true) {
			logFile << "[Success]:" + log + "[" + location + "]\n";
		}
	}
	void LogData(string log, string location) {
		if (logOpen == true) {
			logFile << "[Data]:" + log + "[" + location + "]\n";
		}
	}
	void Log(int code) {
		if (logOpen == true) {
			logFile << "[" + to_string(code) + "]" << "\n";
		}
	}
	void TerminateLogging() {
		LogSuccess("Terminated Log", "Logging.cpp/terminateLogging");
		logFile.close();
	}
}

int level = 0;
unsigned int index = 1;
vector<JSONFile> loadedJSONFiles;
namespace CONCERO {
	void RunScript(vector<string> scriptData)
	{
		if (scriptData[0] == "") {
			scriptData[0] = "GetPage";
		}
		string transferLocation = scriptData[0] + ".tran";
		ofstream transferData(transferLocation.c_str());
		if (transferData.is_open()) {
			for (unsigned a = 1; a < scriptData.size(); a++) {
				if (a != scriptData.size() - 1) {
					transferData << scriptData[a] << endl;
				}
				else {
					transferData << scriptData[a];
				}
			}
			transferData.close();
		}
		else {
			LOGGING::LogError("Unable to genorate transfer data file for script: " + scriptData[0], "Conscientia.cpp/CONCERO/RunScript");
		}
		if (initConscientia == false) {
			string scriptCall = scriptData[0] + ".py";
			system(scriptCall.c_str());
		}
		else {
			LOGGING::LogError("Running python scripts after Conscientia has been initialized is prohibited", "Conscientia.cpp/CONCERO/RunScript");
		}
		cout << "Ran Script\n";
	}
	void InterpretJSON(string file)
	{
		index = 1;
		level = 0;
		string raw = "";
		ifstream load(file.c_str());
		if (load.is_open()) {
			string line;
			while (getline(load, line)) {
				for (unsigned a = 0; a < line.size(); a++) {
					if (line[a] == '[') {
						line[a] = NULL;
					}
					if (line[a] == ']') {
						line[a] = NULL;
					}
				}
				raw = raw + line;
			}
			load.close();
		}
		else {
			cout << ">>>Unable to open: " + file << endl;
		}
		string current;
		JSONFile fileJSON;
		fileJSON.variables.push_back(ReadVar(file, raw));
		loadedJSONFiles.push_back(fileJSON);
	}
	subVar ReadVar(string name, string raw)
	{
		subVar newLevel;
		newLevel.name = name;
		string strLineA, strLineB;
		bool loadName = false, loadVal = false, stringVal = false, action = false, nameStored = false;
		newLevel.name = name;
		for (index; index < raw.size() - 1; index++) {
			//cout << index << ",";
			if (raw[index] == '{') {
				index++;
				string print = "";
				for (int b = 0; b < level; b++) {
					print = print + ">";
				}
				//cout << print << '"' << strLineA << "\":\n";
				level++;
				subVar level = ReadVar(strLineA, raw);
				newLevel.vars.push_back(level);
				index = index + 1;
				strLineA = "";
				strLineB = "";
				loadName = false;
				loadVal = false;
				stringVal = false;
				action = false;
				nameStored = false;
			}
			if (raw[index] == '"' && loadName == false && action == false && nameStored == false) {
				loadName = true;
				action = true;
			}
			else if (raw[index] == '"' && loadName == true && action == false) {
				loadName = false;
				nameStored = true;
			}
			if (raw[index] == ':' && raw[index + 1] != '{' && action == false) {
				loadVal = true;
				action = true;
			}
			if (raw[index] == '\"' && loadVal == true && action == false) {
				stringVal = true;
				action = true;
			}
			if (raw[index] == ',' && loadVal == true || raw[index] == '}' && loadVal == true) {
				subVar newVariable;
				newVariable.name = strLineA;
				newVariable.strVar = strLineB;
				loadVal == false;
				if (stringVal == false) {
					bool doubleCheck = false;
					for (unsigned b = 0; b < strLineB.size(); b++) {
						if (strLineB[b] == '.') {
							doubleCheck = true;
						}
					}
					if (doubleCheck == false) {
						string::size_type returnSize;
						newVariable.intVar = stoi(strLineB, &returnSize);
					}
					else if (doubleCheck == true) {
						string::size_type returnSize;
						newVariable.doubleVar = stod(strLineB, &returnSize);
					}
				}
				else if (stringVal == true) {
					stringVal = false;
				}
				string print = "";
				for (int b = 0; b < level; b++) {
					print = print + ">";
				}
				//cout << print << newVariable.name << ":" << newVariable.strVar << endl;
				newLevel.vars.push_back(newVariable);
				strLineA = "";
				strLineB = "";
				loadName = false;
				loadVal = false;
				stringVal = false;
				action = false;
				nameStored = false;
			}
			if (loadName == true && action == false) {
				strLineA = strLineA + raw[index];
			}
			if (loadVal == true && action == false) {
				strLineB = strLineB + raw[index];
			}
			if (raw[index] == '}' && action == false) {
				level--;
				return(newLevel);
			}
			action = false;
		}
		return (newLevel);
	}
}