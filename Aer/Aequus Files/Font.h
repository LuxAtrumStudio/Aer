#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include "SDL Headers.h"
using namespace std;

struct FONT {
	TTF_Font* fontPointer;
	string name;
	int weight, point;
	bool italic;
	vector<int> avalibaleWeights;
	vector<int> avalibaleItalics;
	string fontFolderPath;
};

namespace TTF {
	extern FONT globalDefaultFont;
	extern string globalFolderPath;
	extern vector<string> globalWeightBindings;
	extern vector<FONT> fontList;
	void SetDefaultFont(string folderPath);
	void LoadFont(string name);
	int GetFontPointer(string name);
	void TerminateFont(int pointer);
	void FTerminateFont(string name);
	void NullFont(int pointer);
	void FNullFont(string name);
	void TerminateAllFonts();
	void SetFontWeight(int pointer, string value);
	void SetFontWeightInt(int pointer, int value);
	void SetFontPoint(int pointer, int value);
	void SetFontItalic(int pointer, bool value);
	void FSetFontWeight(string name, string value);
	void FSetFontWeightInt(string name, int value);
	void FSetFontPoint(string name, int value);
	void FSetFontItalic(string name, bool value);
	void GetTextSize(int pointer, string text, int& x, int& y);
	void FGetTextSize(string name, string text, int& x, int& y);

	void GenFont(FONT& font);
}