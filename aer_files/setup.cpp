#include "aer_headers.h"
#include <aequus.h>
#include <ctime>
#include <iostream>
#include <string>

void aer::setup::SetUp() {
  updatedata = pessum::luxreader::LoadLuxDataFile("ProgramData.lux");
  time_t currenttime, lastupdate;
  lastupdate = (time_t)updatedata.datafilevariables[0].intvalue;
  time(&currenttime);
  tm currentTM;
  tm lastupdateTM;
  currentTM = *gmtime(&currenttime);
  lastupdateTM = *gmtime(&lastupdate);
  if (currentTM.tm_hour != lastupdateTM.tm_hour ||
      currentTM.tm_mday != lastupdateTM.tm_mday ||
      currentTM.tm_mon != lastupdateTM.tm_mon ||
      currentTM.tm_year != lastupdateTM.tm_year ||
      updatedata.datafilevariables[3].stringvectorvalues.size() > 0) {
    std::cout << ">Fetching New Location Data:\n";
    for (unsigned a = 0;
         a < updatedata.datafilevariables[3].stringvectorvalues.size(); a++) {
      std::cout << ">>" + updatedata.datafilevariables[3].stringvectorvalues[a]
                << "\n";
      data::GetLocationData(
          updatedata.datafilevariables[3].stringvectorvalues[a]);
      std::string filename =
          "resources/location/" +
          updatedata.datafilevariables[3].stringvectorvalues[a] + ".json";
      pessum::rest::InterpretJson(filename);
      pessum::rest::JsonFile newlocation = pessum::rest::GetJsonFile(filename);
      pessum::rest::Variable position =
          pessum::rest::GetVariable("centroid", filename);
      double lat, lon;
      lat = position.subvariables[0].doublevalue;
      lon = position.subvariables[1].doublevalue;
      updatedata.datafilevariables[1].stringvectorvalues.push_back(
          updatedata.datafilevariables[3].stringvectorvalues[a]);
      updatedata.datafilevariables[2].doublevectorvalues.push_back(lat);
      updatedata.datafilevariables[2].doublevectorvalues.push_back(lon);
      pessum::logging::Log(
          pessum::logging::LOG_SUCCESS,
          "Loaded new location data for: " +
              updatedata.datafilevariables[3].stringvectorvalues[a],
          "aer.cpp/aer/StartUp");
    }
    updatedata.datafilevariables[3].stringvectorvalues.clear();
    std::cout << ">Fetching Updated Weather Data:\n";
    for (unsigned a = 0;
         a < updatedata.datafilevariables[1].stringvectorvalues.size(); a++) {
      double lat, lon;
      lat = updatedata.datafilevariables[2].doublevectorvalues[a * 2];
      lon = updatedata.datafilevariables[2].doublevectorvalues[(a * 2) + 1];
      std::cout << ">>" << updatedata.datafilevariables[1].stringvectorvalues[a]
                << ":" << lat << "," << lon << "\n";
      data::GetWeatherData(
          lat, lon, updatedata.datafilevariables[1].stringvectorvalues[a]);
      pessum::logging::Log(
          pessum::logging::LOG_SUCCESS,
          "Loaded new weather data for: " +
              updatedata.datafilevariables[1].stringvectorvalues[a],
          "aer.cpp/aer/StartUp");
    }
    updatedata.datafilevariables[0].intvalue = currenttime;
    pessum::luxreader::SaveLuxDataFile("ProgramData.lux", updatedata);
  }
  LoadJsonData();
}

void aer::setup::LoadWindowLayout() {
  pessum::luxreader::DataFile programsettings =
      pessum::luxreader::LoadLuxDataFile(
          "resources/lux_files/program_setting.lux");
  aequus::video::CreateWindow(programsettings.datafilevariables[0].stringvalue,
                              programsettings.datafilevariables[2].intvalue,
                              programsettings.datafilevariables[3].intvalue);
  aequus::video::SetIcon(programsettings.datafilevariables[1].stringvalue);

  aequus::video::NewObject();
  aequus::video::win->obj->CreateImgObj("back_arrow.png");
}

void aer::setup::LoadJsonData() {
  for (unsigned a = 0;
       a < updatedata.datafilevariables[1].stringvectorvalues.size(); a++) {
    pessum::rest::InterpretJson(
        "resources/location/" +
        updatedata.datafilevariables[1].stringvectorvalues[a] + ".json");
    pessum::rest::InterpretJson(
        "resources/weather/" +
        updatedata.datafilevariables[1].stringvectorvalues[a] + ".json");
  }
}
