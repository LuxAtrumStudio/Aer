#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

struct luxCode {
	vector<string> lines;
};
/*Lux Hierarchy*/
struct menuList {
	string name;
	vector<string> items;
};
struct menuPage {
	string name;
	vector<menuList> lists;
};
struct menuHierarchy {
	string name;
	vector<menuPage> pages;
};
/*Lux Data*/
struct dataVariable
{
	string name;
	string type, typeTwo;
	int intValue;
	string stringValue;
	double doubleValue;
	vector<int> intVectorValue;
	vector<string> stringVectorValue;
	vector<double> doubleVectorValue;
};
struct dataFile
{
	string name;
	vector<dataVariable> data;
};

namespace LUXLECTOR {
	dataFile LoadDataFile(string fileDirectory);
	menuHierarchy LoadHierarchyFile(string fileDirectory);
	luxCode GetRawData(int fileType, string fileDirectory);

	void SaveDataFile(string fileDirectory, dataFile outData);
}
