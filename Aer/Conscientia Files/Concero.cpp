#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <ctime>
#include <conio.h>
#include "Conscientia Headers.h"

int layer = 0;
unsigned int index = 1;
vector<JSONFile> loadedJSONFiles;
void CONCERO::RunScript(vector<string> scriptData)
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
	string scriptLocation = scriptData[0] + ".py";
	system(scriptLocation.c_str());
}
void CONCERO::InterpretJSON(string file)
{
	index = 1;
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
	//print(fileJSON.variables[0]);
}
subVar CONCERO::ReadVar(string name, string raw)
{
	subVar newLevel;
	newLevel.name = name;
	string strLineA, strLineB;
	bool loadName = false, loadVal = false, stringVal = false, action = false, nameStored = false;
	newLevel.name = name;
	for (index; index < raw.size() - 1; index++) {
		if (raw[index] == '{') {
			index++;
			string print = "";
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
		if (raw[index] == ':' && raw[index + 1] != '{' && action == false && loadVal == false) {
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
			return(newLevel);
		}
		action = false;
	}
	return (newLevel);
}
JSONFile CONCERO::GetJSONFile(string name)
{
	for (unsigned a = 0; a < loadedJSONFiles.size(); a++) {
		if (loadedJSONFiles[a].variables[0].name == name) {
			return(loadedJSONFiles[a]);
		}
	}
}

subVar CONCERO::LocateVariable(string varName, subVar file)
{
	subVar result, check;
	result.name == "NULL";
	if (varName == file.name) {
		return(file);
	}
	for (unsigned a = 0; a < file.vars.size(); a++) {
		check = LocateVariable(varName, file.vars[a]);
		if (check.name == varName) {
			result = check;
			return(result);
		}
	}
	return (result);
}

subVar CONCERO::GetVariable(string varName, string JSONName)
{
	subVar result;
	JSONFile file = GetJSONFile(JSONName);
	for (unsigned a = 0; a < file.variables.size(); a++) {
		result = LocateVariable(varName, file.variables[a]);
	}
	if (result.name == "NULL") {
		result.intVar = NULL;
		result.doubleVar = NULL;
		result.strVar = "";
		result.vars.clear();
	}
	return (result);
}

int CONCERO::GetIntVariable(string varName, string JSONName)
{
	int value;
	JSONFile file = GetJSONFile(JSONName);
	subVar variable;
	for (unsigned a = 0; a < file.variables.size(); a++) {
		variable = LocateVariable(varName, file.variables[a]);
	}
	if (variable.name == "NULL") {
		variable.intVar = NULL;
		variable.doubleVar = NULL;
		variable.strVar = "";
		variable.vars.clear();
	}
	value = variable.intVar;
	return (value);
}

string CONCERO::GetStringVariable(string varName, string JSONName)
{
	string value;
	JSONFile file = GetJSONFile(JSONName);
	subVar variable;
	for (unsigned a = 0; a < file.variables.size(); a++) {
		variable = LocateVariable(varName, file.variables[a]);
	}
	if (variable.name == "NULL") {
		variable.intVar = NULL;
		variable.doubleVar = NULL;
		variable.strVar = "";
		variable.vars.clear();
	}
	value = variable.strVar;
	return (value);
}

double CONCERO::GetDoubleVariable(string varName, string JSONName)
{
	double value;
	JSONFile file = GetJSONFile(JSONName);
	subVar variable;
	for (unsigned a = 0; a < file.variables.size(); a++) {
		variable = LocateVariable(varName, file.variables[a]);
	}
	if (variable.name == "NULL") {
		variable.intVar = NULL;
		variable.doubleVar = NULL;
		variable.strVar = "";
		variable.vars.clear();
	}
	value = variable.doubleVar;
	return (value);
}

void CONCERO::print(subVar var) {
	for (int a = 0; a < layer; a++) {
		cout << ">";
	}
	cout << var.name << ":";
	if (var.vars.size() > 0) {
		cout << "\n";
		layer++;
		for (unsigned a = 0; a < var.vars.size(); a++) {
			print(var.vars[a]);
		}
	}
	else {
		cout << var.strVar << "\n";
	}
	layer--;
}