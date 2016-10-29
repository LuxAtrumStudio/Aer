#include "aer.h"
#include <aequus.h>
int main() {
  pessum::InitializePessumComponents();
  aequus::framework::SdlStartUp();
  aequus::video::CreateWindow("Aer", 400, 600);
  aequus::video::SetIcon("resources/images/weather_icons/sun-cloud.png");
  aequus::framework::TerminateSdl();
  pessum::TerminatePessumComponents();
}
