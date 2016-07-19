#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

struct subVar {
	string name;
	vector<subVar> vars;
	string strVar;
	int intVar;
	double doubleVar;
};
struct JSONFile {
	vector<subVar> variables;
};
namespace CONCERO {
	void RunScript(vector<string> scriptData);
	void InterpretJSON(string file);
	subVar ReadVar(string name, string raw);
	JSONFile GetJSONFile(string name);

	subVar LocateVariable(string varName, subVar file);
	subVar GetVariable(string varName, string JSONName);
	int GetIntVariable(string varName, string JSONName);
	string GetStringVariable(string varName, string JSONName);
	double GetDoubleVariable(string varName, string JSONName);
}
