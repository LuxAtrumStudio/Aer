#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <ctime>
#include "SDL Headers.h"
#include "../Conscientia Files/Conscientia Headers.h";
#include "Aequus Headers.h"
using namespace std;

namespace TTF {
	FONT globalDefaultFont;
	string globalFolderPath;
	vector<string> globalWeightBindings = { "Thin", "ExtraLight", "Light", "Regular", "Medium", "SemiBold", "Bold", "ExtraBold", "Black" };
	vector<FONT> fontList;
}

void TTF::SetDefaultFont(string folderPath)
{
	globalDefaultFont.fontFolderPath = folderPath;
	globalFolderPath = folderPath;
	globalDefaultFont.italic = false;
	globalDefaultFont.name = "RaleWay";
	globalDefaultFont.point = 24;
	globalDefaultFont.weight = 3;
	globalDefaultFont.avalibaleWeights = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	globalDefaultFont.avalibaleItalics = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	GenFont(globalDefaultFont);
	fontList.push_back(globalDefaultFont);
}

void TTF::LoadFont(string name)
{
	FONT newFont;
	newFont.name = name;
	string directory = globalFolderPath + "/" + name + "/" + name + "-";
	bool italic = false;
	for (int a = 0; a < 9; a++) {
		directory = globalFolderPath + "/" + name + "/" + name + "-";

		directory = directory + globalWeightBindings[a];
		if (italic == true) {
			directory = directory + "Italic";
		}
		directory = directory + ".ttf";
		ifstream fileCheck(directory.c_str());
		if (fileCheck.is_open()) {
			newFont.avalibaleWeights.push_back(a);
			fileCheck.close();
		}

		italic = true;
		directory = globalFolderPath + "/" + name + "/" + name + "-";

		if (globalWeightBindings[a] == "Regular" && italic == true) {
			directory = directory + "Italic";
		}
		directory = directory + globalWeightBindings[a];
		if (italic == true) {
			directory = directory + "Italic";
		}
		directory = directory + ".ttf";
		ifstream fileCheckItalic(directory.c_str());
		if (fileCheckItalic.is_open()) {
			newFont.avalibaleItalics.push_back(a);
			fileCheckItalic.close();
		}
	}
	newFont.fontFolderPath = globalFolderPath;
	newFont.italic = false;
	newFont.point = 24;
	newFont.weight = 3;
	GenFont(newFont);
	fontList.push_back(newFont);
}

int TTF::GetFontPointer(string name)
{
	for (unsigned a = 0; a < fontList.size(); a++) {
		if (fontList[a].name == name) {
			return(a);
		}
	}
	return(0);
}

void TTF::TerminateFont(int pointer)
{
	TTF_CloseFont(fontList[pointer].fontPointer);
	fontList[pointer].fontPointer = NULL;
	fontList.erase(fontList.begin() + pointer);
}

void TTF::FTerminateFont(string name)
{
	TerminateFont(GetFontPointer(name));
}

void TTF::NullFont(int pointer)
{
	TTF_CloseFont(fontList[pointer].fontPointer);
	fontList[pointer].fontPointer = NULL;
}

void TTF::FNullFont(string name)
{
	NullFont(GetFontPointer(name));
}

void TTF::TerminateAllFonts()
{
	for (unsigned a = 0; a < fontList.size(); a++) {
		TerminateFont(a);
	}
}

void TTF::SetFontWeight(int pointer, string value)
{
	int index = 3;
	for (unsigned a = 0; a < globalWeightBindings.size(); a++) {
		if (globalWeightBindings[a] == value) {
			index = a;
		}
	}
	fontList[pointer].weight = index;
	GenFont(fontList[pointer]);
}

void TTF::SetFontWeightInt(int pointer, int value)
{
	fontList[pointer].weight = value;
	GenFont(fontList[pointer]);
}

void TTF::SetFontPoint(int pointer, int value)
{
	fontList[pointer].point = value;
	GenFont(fontList[pointer]);
}

void TTF::SetFontItalic(int pointer, bool value)
{
	fontList[pointer].italic = value;
	GenFont(fontList[pointer]);
}

void TTF::FSetFontWeight(string name, string value)
{
	SetFontWeight(GetFontPointer(name), value);
}

void TTF::FSetFontWeightInt(string name, int value)
{
	SetFontWeightInt(GetFontPointer(name), value);
}

void TTF::FSetFontPoint(string name, int value)
{
	SetFontPoint(GetFontPointer(name), value);
}

void TTF::FSetFontItalic(string name, bool value)
{
	SetFontItalic(GetFontPointer(name), value);
}

void TTF::GetTextSize(int pointer, string text, int & x, int & y)
{
	if (fontList[pointer].fontPointer != NULL) {
		TTF_SizeText(fontList[pointer].fontPointer, text.c_str(), &x, &y);
	}
}

void TTF::FGetTextSize(string name, string text, int & x, int & y)
{
	GetTextSize(GetFontPointer(name), text, x, y);
}

void TTF::GenFont(FONT& font)
{
	string directory;
	directory = font.fontFolderPath + "/" + font.name + "/" + font.name + "-";
	if (font.weight == 3 && font.italic == true) {
		directory = directory + "Italic";
	}
	else {
		directory = directory + globalWeightBindings[font.weight];
		if (font.italic == true) {
			directory = directory + "Italic";
		}
	}
	directory = directory + ".ttf";
	font.fontPointer = TTF_OpenFont(directory.c_str(), font.point);
	if (font.fontPointer == NULL) {
		LOGGING::LogError("Unable to load " + directory, "Font.cpp/TTF/GenFont");
	}
	else {
		LOGGING::LogSuccess("Loaded " + directory, "Font.cpp/TTF/GenFont");
	}
}