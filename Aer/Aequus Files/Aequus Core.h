#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include "SDL Headers.h"

namespace AEQUUS {
	extern vector<WINDOW> graphicalWindows;
	extern WINDOW* window;
	extern int devMode;
	extern int majorVersion, minorVersion;
	extern bool mainLoopQuit;
	extern vector<TextureReturn> objectEvent;
	extern SDL_Event Event;
	/*=====>>>>>-----CORE FUNCTIONS-----<<<<<=====*/
	/*=====>>>>>-----Initialization-----<<<<<=====*/
	/*>>>>>-----INDIVIDUAL-----<<<<<*/
	void InitializeSDL();
	void SetTextureFiltering();
	void InitializeImageLoading();
	void InitializeTTF();
	/*>>>>>-----COMPLEATE-----<<<<<*/
	void InitializeAequus();
	/*=====>>>>>-----Run Time-----<<<<<=====*/
	/*>>>>>-----WINDOWS-----<<<<<*/
	/*>>>>>-----Management-----<<<<<*/
	int FindWindowPointer(string name);
	int WindowCount();
	/*>>>>>-----Initialization-----<<<<<*/
	void InitializeNewWindow(string name);
	/*-----POINTER-----*/
	bool BindWindow(int pointer);
	bool SelectCurrentWindow(int pointer);
	void GenorateWindow(int pointer, int posX, int posY, int width, int height);
	/*-----CURRENT-----*/
	void CGenorateWindow(int posX, int posY, int width, int height);
	/*-----FIND-----*/
	bool BindWindowName(string name);
	bool FSelectCurrentWindow(string name);
	void FGenorateWindow(string name, int posX, int posY, int width, int height);
	/*>>>>>-----Run Time-----<<<<<*/
	/*-----SINGLE-----*/
	void UpdateWindow(int pointer);
	bool CheckWindow(int pointer);
	vector<TextureReturn> EventHandleWindow(int pointer, SDL_Event& SDLEvent);
	/*-----ALL-----*/
	void UpdateAllWindows();
	bool CheckAllWindows();
	vector<TextureReturn> EventHandleAllWindows(SDL_Event& SDLEvent);
	/*>>>>>-----TEXTURES-----<<<<<*/
	/*>>>>>-----Loading-----<<<<<*/
	/*-----TEXTURES-----*/
	void NewTexture(string filePath);
	/*-----TEXT-----*/
	void NewText(string text);
	/*-----BUTTON-----*/
	void NewButton(string text, string filePath, int width, int height);
	/*>>>>>-----Manipulation-----<<<<<*/
	void SetPositional(int pointer, int x, int y, double scaleWidth, double scaleHeight, double angle, int flip);
	void SetColor(int pointer, double red, double green, double blue, double alpha, int blend);
	void SetTextColor(int pointer, double red, double green, double blue);
	/*=====>>>>>-----Termination-----<<<<<=====*/
	/*>>>>>-----INDIVIDUAL-----<<<<<*/
	void TerminateSDL();
	/*>>>>>-----COMPLEATE-----<<<<<*/
	void TerminateAequus();

	/*=====>>>>>-----WRAPPED FUNCTIONS-----<<<<<=====*/
	void FullStatup();
	void FullShutDown();
	void Frame();
}