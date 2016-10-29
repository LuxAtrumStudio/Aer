#ifndef AER_CORE_H_
#define AER_CORE_H_
#include <aequus.h>
#include <ctime>
#include <string>
namespace aer {
namespace data {}
namespace setup {}
extern std::string LocationAPIKey, WeatherAPIKey;
extern int currentlocation;
extern bool day;
extern pessum::luxreader::DataFile updatedata;
extern std::string locationdata, weatherdata;
extern int currentforcast, maxforcast;
extern int displaystartcurrent, displaystartforcast;

void DrawData();
void RunProgram();
}
#endif
