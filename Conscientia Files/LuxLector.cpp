#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <ctime>
#include "Conscientia Headers.h"
using namespace std;

dataFile LUXLECTOR::LoadDataFile(string fileDirectory)
{
	luxCode rawCode = GetRawData(2, fileDirectory);
	dataFile newData;
	newData.name = fileDirectory;
	if (rawCode.lines[0] == "-1") {
		LOGGING::LogError(fileDirectory + " contains the incorrect file designation", "LuxLector.cpp/LUXLECTOR/LoadDataFile");
		return(newData);
	}
	bool readType, readTypeTwo, readName, readValue;
	for (unsigned a = 0; a < rawCode.lines.size(); a++) {
		readType = true;
		readTypeTwo = false;
		readName = false;
		readValue = false;
		string value;
		string currentLine;
		currentLine = rawCode.lines[a];
		dataVariable newVariable;
		for (unsigned b = 0; b < rawCode.lines[a].size(); b++) {
			if (currentLine[b] != ' ') {
				if (readType == true) {
					newVariable.type = newVariable.type + currentLine[b];
				}
				if (readTypeTwo == true) {
					newVariable.typeTwo = newVariable.typeTwo + currentLine[b];
				}
				if (readName == true) {
					newVariable.name = newVariable.name + currentLine[b];
				}
				if (readValue == true) {
					value = value + currentLine[b];
				}
			}
			if (currentLine[b] == ' ') {
				if (readType == true) {
					if (newVariable.type == "vector") {
						readTypeTwo = true;
					}
					else {
						readName = true;
					}
					readType = false;
				}
				else if (readTypeTwo == true) {
					readName = true;
					readTypeTwo = false;
				}
				else if (readName == true) {
					readValue = true;
					readName = false;
				}
				else if (readValue == true) {
					if (newVariable.type == "vector") {
						if (newVariable.typeTwo == "int") {
							newVariable.intVectorValue.push_back(stoi(value));
						}
						if (newVariable.typeTwo == "string") {
							newVariable.stringVectorValue.push_back(value);
						}
						if (newVariable.typeTwo == "double") {
							newVariable.doubleVectorValue.push_back(stod(value));
						}
						value = "";
					}
					else {
						readValue = false;
						if (newVariable.type == "int") {
							newVariable.intValue = stoi(value);
						}
						if (newVariable.type == "string") {
							newVariable.stringValue = value;
						}
						if (newVariable.type == "double") {
							newVariable.doubleValue = stod(value);
						}
					}
				}
			}
		}
		if (value.size() > 0) {
			if (newVariable.type == "vector") {
				if (newVariable.typeTwo == "int") {
					newVariable.intVectorValue.push_back(stoi(value));
				}
				if (newVariable.typeTwo == "string") {
					newVariable.stringVectorValue.push_back(value);
				}
				if (newVariable.typeTwo == "double") {
					newVariable.doubleVectorValue.push_back(stod(value));
				}
				value = "";
			}
			else {
				readValue = false;
				if (newVariable.type == "int") {
					newVariable.intValue = stoi(value);
				}
				if (newVariable.type == "string") {
					newVariable.stringValue = value;
				}
				if (newVariable.type == "double") {
					newVariable.doubleValue = stod(value);
				}
			}
		}
		newData.data.push_back(newVariable);
	}
	return(newData);
}

menuHierarchy LUXLECTOR::LoadHierarchyFile(string fileDirectory)
{
	luxCode rawCode = GetRawData(4, fileDirectory);
	menuHierarchy newHierarchy;
	menuPage newPage;
	menuList newList;
	int currentLevel = 0;
	string currentLine = "";
	string codeLine = "";
	if (rawCode.lines[0] == "-1") {
		LOGGING::LogError(fileDirectory + " contains the incorrect file designation", "LuxLector.cpp/LUXLECTOR/LoadHierarchyFile");
		return(newHierarchy);
	}
	for (unsigned a = 0; a < rawCode.lines.size(); a++) {
		currentLine = rawCode.lines[a];
		codeLine = "";
		if (currentLine[0] == '[') {
			for (unsigned b = 1; b < currentLine.size() && currentLine[b] != ']'; b++) {
				codeLine = codeLine + currentLine[b];
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

luxCode LUXLECTOR::GetRawData(int fileType, string fileDirectory)
{
	string line;
	luxCode rawCode;
	ifstream load(fileDirectory.c_str());
	if (load.is_open()) {
		getline(load, line);
		if (line != "#LISTFILE" && fileType == 1) {
			rawCode.lines.clear();
			rawCode.lines.push_back("-1");
			return(rawCode);
		}
		if (line != "#DATAFILE" && fileType == 2) {
			rawCode.lines.clear();
			rawCode.lines.push_back("-1");
			return(rawCode);
		}
		if (line != "#CODEFILE" && fileType == 3) {
			rawCode.lines.clear();
			rawCode.lines.push_back("-1");
			return(rawCode);
		}
		if (line != "#HIERARCHYFILE" && fileType == 4) {
			rawCode.lines.clear();
			rawCode.lines.push_back("-1");
			return(rawCode);
		}
		while (getline(load, line)) {
			rawCode.lines.push_back(line);
		}
		load.close();
	}
	else {
		LOGGING::LogError("Unable to open: " + fileDirectory, "LuxLector.cpp/LUXLECTOR/GetRawData");
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
	return(rawCode);
}

void LUXLECTOR::SaveDataFile(string fileDirectory, dataFile outData)
{
	ofstream outputData(fileDirectory);
	outputData << "#DATAFILE";
	for (unsigned a = 0; a < outData.data.size(); a++) {
		outputData << "\n";
		outputData << outData.data[a].type << " ";
		if (outData.data[a].type == "vector") {
			outputData << outData.data[a].typeTwo << " ";
		}
		outputData << outData.data[a].name;
		if (outData.data[a].type == "int") {
			outputData << " " << to_string(outData.data[a].intValue);
		}
		if (outData.data[a].type == "string") {
			outputData << " " << outData.data[a].stringValue;
		}
		if (outData.data[a].type == "double") {
			outputData << " " << to_string(outData.data[a].doubleValue);
		}
		if (outData.data[a].type == "vector") {
			if (outData.data[a].typeTwo == "int") {
				for (unsigned b = 0; b < outData.data[a].intVectorValue.size(); b++) {
					outputData << " " << to_string(outData.data[a].intVectorValue[b]);
				}
			}
			if (outData.data[a].typeTwo == "string") {
				for (unsigned b = 0; b < outData.data[a].stringVectorValue.size(); b++) {
					outputData << " " << outData.data[a].stringVectorValue[b];
				}
			}
			if (outData.data[a].typeTwo == "double") {
				for (unsigned b = 0; b < outData.data[a].doubleVectorValue.size(); b++) {
					outputData << " " << to_string(outData.data[a].doubleVectorValue[b]);
				}
			}
		}
	}
}
