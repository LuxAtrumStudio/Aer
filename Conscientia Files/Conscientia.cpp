#include <iostream>
#include <string>
#include <sstream>
#include <ncurses.h>
#include <vector>
#include "Conscientia Headers.h"
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
/*>>>>>-----FUNCTIONS-----<<<<<*/
/*>>>>>-----Menu-----<<<<<*/
int firstPage = 0, firstList = 0, firstItem = 0;
/*>>>>>-----Loading Bars-----<<<<<*/
vector<int> loadingBarPointers;

/*=====>>>>>-----FUNCTIONS-----<<<<<=====*/
/*=====>>>>>-----Initilization-----<<<<<=====*/
void CONSCIENTIA::InitializeConscientia(){
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
void CONSCIENTIA::AdvancedInit(bool cursor, bool echo, bool raw){
  initializeConscientia();
  setCursor(cursor);
  setEcho(echo);
  setRaw(raw);
}
/*>>>>-----SETTINGS-----<<<<<*/
void CONSCIENTIA::SetAutoRefresh(bool setting){
  autoRefresh = setting;
}
/*>>>>>----NCURSES INITILIZATION-----<<<<<*/
void CONSCIENTIA::SetCursor(bool setting){
  curs_set(setting);
}
void CONSCIENTIA::SetEcho(bool setting){
  if(setting == true){
      echo();
  }
  else{
      noecho();
  }
}
void CONSCIENTIA::SetRaw(bool setting){
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
void CONSCIENTIA::GenorateWindow(string name, int posX, int posY, int sizeX, int sizeY, bool border, bool title){
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
void CONSCIENTIA::ClearAllWindows(){
  for(unsigned a = 0; a < windows.size(); a++){
    clearWindow(a);
  }
}
int CONSCIENTIA::FindWindowPointer(string name){
    for(unsigned a = 0; a < windows.size(); a++){
      if(windows[a].name == name){
        return(a);
      }
    }
    return(0);
}
/*-----POINTER-----*/
void CONSCIENTIA::SetBorder(int pointer, bool setting){
  if(windows[pointer].border != setting){
    windows[pointer].border = setting;
    drawBorder(pointer, setting);
  }
}
void CONSCIENTIA::ClearWindow(int pointer){
  wclear(windows[pointer].pointer);
}
void CONSCIENTIA::SetWindowTitle(int pointer, bool setting){
  if(windows[pointer].title != setting){
    windows[pointer].title = setting;
    drawTitle(pointer);
  }
}
void CONSCIENTIA::SetCurrentWindow(int pointer){
  currentWindowPointer = pointer;
}
void CONSCIENTIA::GetWindowSize(int pointer, int& x, int &y){
  x = windows[pointer].sizeX;
  y = windows[pointer].sizeY;
}
/*-----CURRENT-----*/
void CONSCIENTIA::CSetBorder(bool setting){
  setBorder(currentWindowPointer, setting);
}
void CONSCIENTIA::CClearWindow(){
  clearWindow(currentWindowPointer);
}
void CONSCIENTIA::CSetWindowTitle(bool setting){
  setWindowTitle(currentWindowPointer, setting);
}
void CONSCIENTIA::CGetWindowsize(int&x, int &y){
  x = windows[currentWindowPointer].sizeX;
	y = windows[currentWindowPointer].sizeY;
}
/*-----FIND-----*/
void CONSCIENTIA::FsetBorder(string name, bool setting){
  setBorder(findWindowPointer(name), setting);
}
void CONSCIENTIA::FClearWindow(string name){
  clearWindow(findWindowPointer(name));
}
void CONSCIENTIA::FSetWindowtitle(string name, bool setting){
  setWindowTitle(findWindowPointer(name), setting);
}
void CONSCIENTIA::FSetCurrentWindow(string name){
  currentWindowPointer = findWindowPointer(name);
}
void CONSCIENTIA::FGetWindowSize(string name, int& x, int& y) {
	x = windows[FindWindowPointer(name)].sizeX;
	y = windows[FindWindowPointer(name)].sizeY;
}
/*-----LINUX-----*/
void CONSCIENTIA::DrawBorder(int pointer, bool setting){
  if(setting == false){
    wborder(windows[pointer].pointer, ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ');
  }
  else if(setting == true){
    box(windows[pointer].pointer, 0, 0);
  }
}
void CONSCIENTIA::DrawTitle(int pointer){
  int titleSize = windows[pointer].name.size();
  int windowSize = windows[pointer].sizeX;
  int posx;
  windowSize = windowSize / 2;
  titleSize = titleSize / 2;
  posx = windowSize - titleSize;
  mvwprintw(windows[pointer].pointer, 0, posx, windows[pointer].name.c_str());
}
/*>>>>>-----Termination-----<<<<<*/
void CONSCIENTIA::TerminateAllWindows(){
  for(unsigned a = 0; a < windows.size(); a++){
    terminateWindow(a);
  }
}
void CONSCIENTIA::TerminateWindow(int pointer){
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
char CONSCIENTIA::Gchar(){
  char in;
  in = getch();
  return(in);
}
int CONSCIENTIA::Gint(){
  int in;
  in = int(getch());
  return(in);
}
int CONSCIENTIA::Cint(){
  int in = 0;
  int rawin = 0;
  while(rawin != 13){
    rawin = int(getch());
    in = (in * 10) + rawin;
  }
  return(in);
}
string CONSCIENTIA::Cstr(){
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
float CONSCIENTIA::Cfloat(){
  string rawstr = cstr();
  string::size_type sz;
  float in;
  in = stof(rawstr, &sz);
  return(in);
}
/*>>>>>-----Output-----<<<<<*/
/*-----POINTER-----*/
void CONSCIENTIA::Print(int pointer, string str){
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
void CONSCIENTIA::MPrint(int pointer, int x, int y, string str){
  windows[pointer].cursorX = x;
  windows[pointer].cursorY = y;
  print(pointer, str);
}
/*-----CURRENT-----*/
void CONSCIENTIA::CPrint(string str){
  print(currentWindowPointer, str);
}
void CONSCIENTIA::CMPrint(int x, int y, string str){
  mprint(currentWindowPointer, x, y, str);
}
/*-----FIND-----*/
void CONSCIENTIA::FPrint(string name, string str){
  print(findWindowPointer(name), str);
}
void CONSCIENTIA::FMPrint(string name, int x, int y, string str){
  int pointer = findWindowPointer(name);
  mprint(pointer, x, y, str);
}
/*>>>>>-----SYSTEM-----<<<<<*/
/*>>>>>-----Update-----<<<<<*/
void CONSCIENTIA::Update(){
  for(unsigned a = 0; a < windows.size(); a++){
    wrefresh(windows[a].pointer);
  }
}
/*>>>>>-----Console-----<<<<<*/
void SetConsoleName(string title){

}
/*=====>>>>>-----Termination-----<<<<<=====*/
void CONSCIENTIA::TerminateConscientia(){
  endwin();
}
/*=====>>>>>-----ADVANCED FUNCITONS-----<<<<<=====*/
/*=====>>>>>-----Output Funcitons-----<<<<<=====*/
/*>>>>>-----INTERACTIVE-----<<<<<*/
/*>>>>>-----Menu-----<<<<<*/
string CONSCIENTIA::Menu(string menuFileDirectory, int posX, int posY, int sizeX, int sizeY) {
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
void CONSCIENTIA::DisplayMenu(menuHierarchy menu, int currentPage, int currentList, int currentItem) {
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
	createWindow(process, (windows[0].sizeX / 2) - (windows[0].sizeX / 4), (windows[0].sizeY / 2) - 1, (windows[0].sizeX / 2), 3, true, true);
	return(loadingBarPointers.size() - 1);
}
void CONSCIENTIA::LoadingBar(int index, double percent) {
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
void CONSCIENTIA::TerminateLoadingBar(int index) {
	terminateWindow(loadingBarPointers[index]);
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
