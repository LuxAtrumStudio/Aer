#ifndef AER_H_
#define AER_H_
#include <aequus.h>
#include <ctime>
#include <string>
namespace aer {
extern std::string LocaitonAPIKey, WeatherAPIKey;

extern int currentlocation;
extern bool day;
extern pessum::luxreader::DataFile updatedata;
extern std::string locationdata, weatherdata;
extern int currentforcast, maxforcast;
extern int displaystartcurrent, displaystartforcast;

void GetLocationData(std::string location);
void GetWeatherData(double lat, double lon, std::string location);
void SetUp();
void LoadWindowLayout();
void LoadJsonData();
void DrawData();
std::string GetDate(tm date);
std::string GetTime(tm date, bool meridies, bool seconds);
std::string ConvertVartName(std::string var);
std::string ConvertVar(pessum::rest::Variable var);
void LoadCurrentData();
void RunProgram();
void NewLocation();
void RemoveLocation();
}
#endif
