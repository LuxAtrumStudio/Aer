#include "aer_files/aer_headers.h"
#include <aequus.h>
int main() {
  pessum::InitializePessumComponents();
  aequus::framework::SdlStartUp();
  aer::setup::LoadWindowLayout();
  aequus::framework::TerminateSdl();
  pessum::TerminatePessumComponents();
}
