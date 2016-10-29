#ifndef _AER_DATA_H_
#define _AER_DATA_H_
#include <aequus.h>
#include <string>
namespace aer {
namespace data {
void GetLocationData(std::string location);
void GetWeatherData(double lat, double lon, std::string location);
std::string GetDate(tm date);
std::string GetTime(tm date, bool meridies, bool seconds);
std::string ConvertVartName(std::string var);
std::string ConvertVar(pessum::rest::Variable var);
void LoadCurrentData();
void NewLocation();
void RemoveLocation();
}
}
#endif
