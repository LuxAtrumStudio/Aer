
#include "aer_core.h"
#include <aequus.h>
#include <ctime>
#include <iostream>
#include <string>
namespace aer {
std::string LocationAPIKey = "AIzaSyA5f4p18rQqX_7aObYpLJJFV4iqU2-cmf4";
std::string WeatherAPIKey = "9c9c48f9ab5717ed899e8b6d730883c6";

int currentlocation = 0;
bool day = true;
pessum::luxreader::DataFile updatedata;
std::string locationdata, weatherdata;
int currentforcast = 0, maxforcast = 0;
int displaystartcurrent = 0, displaystartforcast = 0;
}

void aer::DrawData() {}

void aer::RunProgram() {
  while (aequus::video::AllClose() == false) {
    aequus::Frame();
    if (aequus::input::events.size() > 0) {
      if (aequus::input::events[0].type == aequus::input::KEYBOARD) {
        if (aequus::input::events[0].key == int('q')) {
          aequus::video::TerminateWindow();
        }
      }
    }
  }
}
