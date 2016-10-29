#include "aer_headers.h"
#include <aequus.h>
#include <ctime>
#include <iostream>
#include <string>

void aer::data::GetLocationData(std::string location) {
  std::vector<std::string> scriptdata = {
      "resources/scripts/get_page.py",
      "https://maps.googleapis.com/maps/api/geocode/"
      "json?address=" +
          location + "&key=" + aer::LocationAPIKey,
      "resources/location/" + location + ".json"};
  pessum::rest::RunScript(scriptdata);
}

void aer::data::GetWeatherData(double lat, double lon, std::string location) {
  std::vector<std::string> scriptdata = {
      "resources/scripts/get_page.py",
      "https://api.darksky.net/forecast/" + WeatherAPIKey + "/" +
          std::to_string(lat) + "," + std::to_string(lon),
      "resources/weather/" + location + ".json"};
  pessum::rest::RunScript(scriptdata);
}

std::string aer::data::GetDate(tm date) {
  std::string dateString = "";
  std::string wDays[7] = {"Sunday",   "Monday", "Tuesday", "Wensday",
                          "Thrusday", "Friday", "Saturday"};
  std::string months[12] = {"January",   "Febuary", "March",    "April",
                            "May",       "June",    "July",     "August",
                            "September", "October", "November", "December"};
  dateString = wDays[date.tm_wday] + " " + months[date.tm_mon] + " " +
               std::to_string(date.tm_mday) + " " +
               std::to_string(1900 + date.tm_year);
  return (dateString);
}

std::string aer::data::GetTime(tm date, bool meridies, bool seconds) {
  std::string timestr;
  bool pm = false;
  if (meridies == true && date.tm_hour > 12) {
    date.tm_hour = date.tm_hour - 12;
    pm = true;
  }
  timestr = std::to_string(date.tm_hour) + ":";
  if (date.tm_min < 10) {
    timestr = timestr + "0" + std::to_string(date.tm_min);
  } else {
    timestr = timestr + std::to_string(date.tm_min);
  }
  if (seconds == true) {
    timestr = timestr + ":" + std::to_string(date.tm_sec);
  }
  if (meridies == true) {
    if (pm == false) {
      timestr = timestr + " AM";
    }
    if (pm == true) {
      timestr = timestr + " PM";
    }
  }
  return (timestr);
}

std::string aer::data::ConvertVartName(std::string var) {
  std::string name;
  name = char(int(var[0]) - 32);
  for (unsigned a = 1; a < var.size() - 1; a++) {
    if (int(var[a]) < 97) {
      name = name + ' ';
    }
    if (var[a] != '-') {
      name = name + var[a];
    }
  }
  name = name + var[var.size() - 1];
  return (name);
}

std::string aer::data::ConvertVar(pessum::rest::Variable var) {
  std::string variable = "";
  if (var.variablename == "time") {
    time_t dataTime = (time_t)var.intvalue;
    int offset = pessum::rest::GetIntVariable("offset", weatherdata);
    dataTime = dataTime + (offset * 3600);
    tm dataTM = *gmtime(&dataTime);
    variable = GetDate(dataTM) + " " + GetTime(dataTM, true, true);
  } else if (var.variablename == "nearestStormDistance" ||
             var.variablename == "visibility") {
    variable = std::to_string(var.intvalue) + " mi";
  } else if (var.variablename == "nearestStormBearing" ||
             var.variablename == "windBearing") {
    std::string bearings[16] = {
        "North", "North, North East", "North East", "East, North East",
        "East",  "East, South East",  "South East", "South, South East",
        "South", "South, South West", "South West", "West, South West",
        "West",  "West, North West",  "North West", "North, North West"};
    int bearing = (int)((var.intvalue / 22.5) + 0.5);
    variable = bearings[bearing % 16];
  } else if (var.variablename == "precipIntensity" ||
             var.variablename == "precipIntensityMax") {
    variable = var.stringvalue + " in/hr";
  } else if (var.variablename == "precipProbability" ||
             var.variablename == "humidity" ||
             var.variablename == "cloudCover") {
    if (var.stringvalue == "0") {
      variable = "0%";
    } else {
      variable = std::to_string((var.doublevalue * (double)100)) + "%";
    }
  } else if (var.variablename == "moonPhase") {
    if (var.stringvalue == "0") {
      variable = "0%";
    } else {
      double phasePercent = var.doublevalue - 0.5;
      if (phasePercent < 0) {
        variable = "Waxing: " +
                   std::to_string(((var.doublevalue * -1) * (double)50)) + "%";
      } else if (phasePercent > 0) {
        variable =
            "Waning: " + std::to_string((var.doublevalue * (double)50)) + "%";
      } else if (phasePercent == 0) {
        variable = "Full Moon: 100%";
      }
    }
  } else if (var.variablename == "temperature" ||
             var.variablename == "apparentTemperature" ||
             var.variablename == "dewPoint" ||
             var.variablename == "temperatureMin" ||
             var.variablename == "temperatureMax" ||
             var.variablename == "apparentTemperatureMax" ||
             var.variablename == "apparentTemperatureMin") {
    variable = var.stringvalue + char(248) + "F";
  } else if (var.variablename == "windSpeed") {
    variable = var.stringvalue + " mph";
  } else if (var.variablename == "pressure") {
    variable = std::to_string(var.doublevalue * 0.000986923) + " atm";
  } else if (var.variablename == "ozone") {
    variable = var.stringvalue + " DU";
  }

  else if (var.variablename == "sunriseTime" ||
           var.variablename == "sunsetTime") {
    time_t dataTime = (time_t)var.intvalue;
    int offset = pessum::rest::GetIntVariable("offset", weatherdata);
    dataTime = dataTime + (offset * 3600);
    tm dataTM = *gmtime(&dataTime);
    variable = GetTime(dataTM, true, true);
  } else if (var.variablename == "temperatureMinTime" ||
             var.variablename == "temperatureMaxTime" ||
             var.variablename == "apparentTemperatureMinTime" ||
             var.variablename == "apparentTemperatureMaxTime" ||
             var.variablename == "precipIntensityMaxTime") {
    time_t dataTime = (time_t)var.intvalue;
    int offset = pessum::rest::GetIntVariable("offset", weatherdata);
    dataTime = dataTime + (offset * 3600);
    tm dataTM = *gmtime(&dataTime);
    variable = GetTime(dataTM, true, false);
  }

  else if (var.variablename == "precipType" || var.variablename == "icon") {
    variable = ConvertVartName(var.stringvalue);
  } else {
    variable = var.stringvalue;
  }
  return (variable);
}

void aer::data::LoadCurrentData() {
  locationdata =
      "resources/location/" +
      updatedata.datafilevariables[1].stringvectorvalues[currentlocation] +
      ".json";
  weatherdata =
      "resources/weather/" +
      updatedata.datafilevariables[1].stringvectorvalues[currentlocation] +
      ".json";
}

void aer::data::NewLocation() {}
void aer::data::RemoveLocation() {}
