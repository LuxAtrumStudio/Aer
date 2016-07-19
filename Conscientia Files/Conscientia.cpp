#include <iostream>
#include <string>
#include <sstream>
#include <ncurses.h>
#include <vector>
#include "Conscientia.h"
using namespace std;

/*=====>>>>>-----DATA-----<<<<<=====*/
struct window {
  string name;
  WINDOW * pointer;
  int startX, startY, sizeX, sizeY, endX, endY;
  int cursorX, cursorY;
  bool border, title;
};
vector<window> windows;
int currentWindowPointer = 0;
bool autoRefresh = false;
/*=====>>>>>-----ADVNACED DATA-----<<<<<=====*/
/*=====>>>>>-----Global-----<<<<<=====*/
struct luxCode {
  vector<string> lines;
};
/*>>>>>-----FUNCTIONS-----<<<<<*/
/*>>>>>-----Menu-----<<<<<*/
int firstPage = 0, firstList = 0, firstItem = 0;
/*>>>>>-----Loading Bars-----<<<<<*/
vector<int> loadingBarPointers;
namespace CONSCIENTIA
{
  /*=====>>>>>-----FUNCTIONS-----<<<<<=====*/
  /*=====>>>>>-----Initilization-----<<<<<=====*/
  void initializeConscientia(){
    initscr();
    keypad(stdscr, TRUE);
    window declaration;
    declaration.name = "stdscr";
    declaration.pointer = stdscr;
    declaration.startX = 0;
    declaration.startY = 0;
    getmaxyx(stdscr, declaration.sizeY, declaration.sizeX);
    declaration.cursorX = 0;
    declaration.cursorY = 0;
    declaration.endX = declaration.sizeX;
    declaration.endY = declaration.sizeY;
    declaration.border = false;
    declaration.title = false;
    windows.push_back(declaration);
    setCursor(false);
    setEcho(false);
  }
  void advancedInit(bool cursor, bool echo, bool raw){
    initializeConscientia();
    setCursor(cursor);
    setEcho(echo);
    setRaw(raw);
  }
  /*>>>>-----SETTINGS-----<<<<<*/
  void setAutoRefresh(bool setting){
    autoRefresh = setting;
  }
  /*>>>>>----NCURSES INITILIZATION-----<<<<<*/
  void initilizeColors(){
    start_color();
  }
  void setCursor(bool setting){
    curs_set(setting);
  }
  void setEcho(bool setting){
    if(setting == true){
        echo();
    }
    else{
        noecho();
    }
  }
  void setRaw(bool setting){
    if(setting == true){
        raw();
    }
    else{
        noraw();
    }
  }
  /*=====>>>>>-----Run Time-----<<<<<=====*/
  /*>>>>>-----WINDOW-----<<<<<*/
  /*>>>>>-----Initilization-----<<<<<*/
  void createWindow(string name, int posX, int posY, int sizeX, int sizeY, bool border, bool title){
    WINDOW *local;
    local = newwin(sizeY, sizeX, posY, posX);
    keypad(local, TRUE);
    if (border == true){
      box(local, 0, 0);
    }
    window declaration;
    declaration.name = name;
    declaration.pointer = local;
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
    if(border == true){
      declaration.cursorX++;
      declaration.cursorY++;
    }
    if(title == true && border == false){
      declaration.cursorY++;
    }
    windows.push_back(declaration);
    if(title == true){
      drawTitle(windows.size() - 1);
    }
  }
  /*>>>>>-----Management-----<<<<<*/
  void clearAllWindows(){
    for(unsigned a = 0; a < windows.size(); a++){
      clearWindow(a);
    }
  }
  int findWindowPointer(string name){
      for(unsigned a = 0; a < windows.size(); a++){
        if(windows[a].name == name){
          return(a);
        }
      }
      return(0);
  }
  /*-----POINTER-----*/
  void setBorder(int pointer, bool setting){
    if(windows[pointer].border != setting){
      windows[pointer].border = setting;
      drawBorder(pointer, setting);
    }
  }
  void clearWindow(int pointer){
    wclear(windows[pointer].pointer);
  }
  void setWindowTitle(int pointer, bool setting){
    if(windows[pointer].title != setting){
      windows[pointer].title = setting;
      drawTitle(pointer);
    }
  }
  void setCurrentWindow(int pointer){
    currentWindowPointer = pointer;
  }
  /*-----CURRENT-----*/
  void csetBorder(bool setting){
    setBorder(currentWindowPointer, setting);
  }
  void cclearWindow(){
    clearWindow(currentWindowPointer);
  }
  void csetWindowTitle(bool setting){
    setWindowTitle(currentWindowPointer, setting);
  }
  /*-----FIND-----*/
  void fsetBorder(string name, bool setting){
    setBorder(findWindowPointer(name), setting);
  }
  void fclearWindow(string name){
    clearWindow(findWindowPointer(name));
  }
  void fsetWindowtitle(string name, bool setting){
    setWindowTitle(findWindowPointer(name), setting);
  }
  void setCurrentWindow(string name){
    currentWindowPointer = findWindowPointer(name);
  }
  /*-----LINUX-----*/
  void drawTitle(int pointer){
    int titleSize = windows[pointer].name.size();
    int windowSize = windows[pointer].sizeX;
    int posx;
    windowSize = windowSize / 2;
    titleSize = titleSize / 2;
    posx = windowSize - titleSize;
    mvwprintw(windows[pointer].pointer, 0, posx, windows[pointer].name.c_str());
  }
  void drawBorder(int pointer, bool setting){
    if(setting == false){
      wborder(windows[pointer].pointer, ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ');
    }
    else if(setting == true){
      box(windows[pointer].pointer, 0, 0);
    }
  }
  /*>>>>>-----Termination-----<<<<<*/
  void terminateAllWindows(){
    for(unsigned a = 0; a < windows.size(); a++){
      terminateWindow(a);
    }
  }
  void terminateWindow(int pointer){
    if(windows[pointer].border == true){
      setBorder(pointer, false);
    }
    if(windows[pointer].title == true){
      setWindowTitle(pointer, false);
    }
    clearWindow(pointer);
    windows.erase(windows.begin() + pointer);
  }
  /*>>>>>-----USER INTERFACE-----<<<<<*/
  /*>>>>>-----Input-----<<<<<*/
  char gchar(){
    char in;
    in = getch();
    return(in);
  }
  int gint(){
    int in;
    in = int(getch());
    return(in);
  }
  int cint(){
    int in = 0;
    int rawin = 0;
    while(rawin != 13){
      rawin = int(getch());
      in = (in * 10) + rawin;
    }
    return(in);
  }
  string cstr(){
    string in;
    char inch;
    int rawint = 0;
    while(rawint != 13){
      rawint = int(getch());
      inch = char(rawint);
      in = in + inch;
    }
    return(in);
  }
  float cfloat(){
    string rawstr = cstr();
    string::size_type sz;
    float in;
    in = stof(rawstr, &sz);
    return(in);
  }
  /*>>>>>-----Output-----<<<<<*/
  /*-----POINTER-----*/
  void print(int pointer, string str){
    for(int a = 0; a < str.size(); a++){
      if(str[a] == '/'){
        int b = a + 1;
        if(str[b] == 'n'){
          windows[pointer].cursorY++;
          windows[pointer].cursorX = 0;
          if(windows[pointer].cursorY > windows[pointer].sizeY){
            windows[pointer].cursorY = 0;
            if(windows[pointer].border == true){
              windows[pointer].cursorY++;
              windows[pointer].cursorX++;
            }
            if(windows[pointer].border == false && windows[pointer].title == true){
              windows[pointer].cursorY++;
            }
            clearWindow(pointer);
          }
        }
        a = a + 2;
      }
      else if(str[a] != '/'){
        if(windows[pointer].border == false){
          if(windows[pointer].cursorX >= windows[pointer].sizeX){
            windows[pointer].cursorX = 0;
            windows[pointer].cursorY++;
            if(windows[pointer].cursorY >= windows[pointer].sizeY){
              windows[pointer].cursorY = 0;
              if(windows[pointer].title == true){
                windows[pointer].cursorY++;
              }
              clearWindow(pointer);
            }
          }
        }
        if(windows[pointer].border == true){
          if(windows[pointer].cursorX >= windows[pointer].sizeX - 1){
            windows[pointer].cursorX = 1;
            windows[pointer].cursorY++;
            if(windows[pointer].cursorY >= windows[pointer].sizeY - 1){
              windows[pointer].cursorY = 1;
              clearWindow(pointer);
            }
          }
        }
        string character;
        stringstream sstream;
        sstream << str[a];
        sstream >> character;
        mvwprintw(windows[pointer].pointer, windows[pointer].cursorY, windows[pointer].cursorX, character.c_str());
        windows[pointer].cursorX++;
      }
    }
    if(autoRefresh == true){
      update();
    }
  }
  void mprint(int pointer, int x, int y, string str){
    windows[pointer].cursorX = x;
    windows[pointer].cursorY = y;
    print(pointer, str);
  }
  /*-----CURRENT-----*/
  void cprint(string str){
    print(currentWindowPointer, str);
  }
  void cmprint(int x, int y, string str){
    mprint(currentWindowPointer, x, y, str);
  }
  /*-----FIND-----*/
  void fprint(string name, string str){
    print(findWindowPointer(name), str);
  }
  void fmprint(string name, int x, int y, string str){
    int pointer = findWindowPointer(name);
    mprint(pointer, x, y, str);
  }
  /*>>>>>-----SYSTEM-----<<<<<*/
  /*>>>>>-----Update-----<<<<<*/
  void update(){
    for(unsigned a = 0; a < windows.size(); a++){
      wrefresh(windows[a].pointer);
    }
  }
  /*=====>>>>>-----Termination-----<<<<<=====*/
  void terminateConscientia(){
    endwin();
  }
  /*=====>>>>>-----ADVANCED FUNCITONS-----<<<<<=====*/
	/*=====>>>>>-----Output Funcitons-----<<<<<=====*/
	/*>>>>>-----INTERACTIVE-----<<<<<*/
	/*>>>>>-----Menu-----<<<<<*/
	string menu(string menuFileDirectory, int posX, int posY, int sizeX, int sizeY) {
		bool run = true, update = true;
		int in = -1;
		menuHierarchy menuStruct;
		menuStruct = loadMenuHierarchy(menuFileDirectory);
		int windowPointer = windows.size();
		int pageWidth, listWidth;
		int currentPage = 0, currentList = 0, currentItem = 0;
		createWindow(menuStruct.name, posX, posY, sizeX, sizeY, true, true);
		setCurrentWindow(windowPointer);
		while (run == true) {
			if (update == true) {
				update = false;
				displayMenu(menuStruct, currentPage, currentList, currentItem);
        drawBorder(currentWindowPointer, true);
        drawTitle(currentWindowPointer);
        CONSCIENTIA::update();
			}
			in = gint();
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
				terminateWindow(currentWindowPointer);
				return(menuStruct.pages[currentPage].lists[currentList].items[currentItem]);
			}
		}
		return("");
	}
	menuHierarchy loadMenuHierarchy(string menuFileDirectory) {
		string line;
		luxCode rawCode;
		int lineCount = 0, totalLines;
		int loadBar = -1;
		ifstream load(menuFileDirectory.c_str());
		if (load.is_open()) {
			load >> lineCount;
			totalLines = lineCount;
			if (lineCount >= 50) {
				loadBar = initializeLoadingBar("Loading Menu");
			}
			lineCount = 0;
			getline(load, line);
			while (getline(load, line)) {
				rawCode.lines.push_back(line);
				lineCount++;
				if (loadBar != -1 && (lineCount % 10) == 0) {
					loadingBar(loadBar, ((double)lineCount / (double)totalLines) * 100);
				}
			}
			load.close();
			if (loadBar != -1) {
				terminateLoadingBar(loadBar);
			}
		}
		string cleanLine;
		bool tabSpace;
		for (unsigned a = 0; a < rawCode.lines.size(); a++) {
			line = rawCode.lines[a];
			cleanLine = "";
			tabSpace = true;
			for (unsigned b = 0; b < line.size(); b++) {
				if (tabSpace == true && line[b] != ' ') {
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
	void displayMenu(menuHierarchy menu, int currentPage, int currentList, int currentItem) {
		cclearWindow();
		string v, h, i, line;
		char vc, hc, ic;
		stringstream vss, hss, iss;
		//vc = char(179);
    //hc = char(196);
		//ic = char(193);
    vc = '|';
    hc = '-';
    ic = '+';
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
			cmprint(x + findTextStart(menu.pages[a].name, pageWidth), y, menu.pages[a].name);
			if (currentPage != a) {
				cmprint(x + 1, y + 1, line);
			}
			x = x + pageWidth;
			if (a != pagesDisplayed - 1) {
				cmprint(x, y, v);
				cmprint(x, y + 1, i);
			}
		}
		x = 0;
		y = y + 2;
		for (int a = firstList; a < listsDisplayed + firstList; a++) {
			cmprint(x + findTextStart("<" + menu.pages[currentPage].lists[a].name + ">", listWidth), y, "<" + menu.pages[currentPage].lists[a].name + ">");
			yn = y + 1;
			for (int b = firstItem; b < maxItemsDisplayed + firstItem && b < menu.pages[currentPage].lists[a].items.size(); b++) {
				if (a == currentList && b == currentItem) {
					cmprint(x + findTextStart(">" + menu.pages[currentPage].lists[a].items[b] + "<", listWidth), yn, ">" + menu.pages[currentPage].lists[a].items[b] + "<");
				}
				else {
					cmprint(x + findTextStart(menu.pages[currentPage].lists[a].items[b], listWidth), yn, menu.pages[currentPage].lists[a].items[b]);
				}
				yn++;
			}
			x = x + listWidth;
		}
		update();
	}
	int findTextStart(string str, int space) {
		space = space / 2;
		int strSize = str.size();
		strSize = strSize / 2;
		int strStart = space - strSize;
		return(strStart);
	}
	/*>>>>>-----DISPLAY-----<<<<<*/
	/*>>>>>-----Loading Bars-----<<<<<*/
	int initializeLoadingBar(string process) {
		loadingBarPointers.push_back(windows.size());
		createWindow(process, (windows[0].sizeX / 2) - (windows[0].sizeX / 4), (windows[0].sizeY / 2) - 1, (windows[0].sizeX / 2), 3, true, true);
		return(loadingBarPointers.size() - 1);
	}
	void loadingBar(int index, double percent) {
		char block = char(219);
		int loadingBarPointer = loadingBarPointers[index];
		int size = windows[loadingBarPointer].sizeX - 2;
		double blockWorth = (double)100 / (double)size;
		clearWindow(loadingBarPointer);
		string bar = "";
		while (percent > blockWorth) {
			bar = bar + block;
			percent = percent - blockWorth;
		}
		print(loadingBarPointer, bar);
	}
	void terminateLoadingBar(int index) {
		terminateWindow(loadingBarPointers[index]);
		loadingBarPointers.erase(loadingBarPointers.begin() + index);
	}
	/*=====>>>>>-----Input Funcitons-----<<<<<=====*/
	/*=====>>>>>-----System Funcitons-----<<<<<=====*/
};
