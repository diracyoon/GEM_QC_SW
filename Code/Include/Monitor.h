#ifndef __Monitor_h__
#define __Monitor_h__

#include <chrono>
#include <thread>

#include <TSystem.h>
#include <TCanvas.h>

#include "Watcher.h"
#include "Painter.h"

#define NUM_CHANNEL 4

using namespace std;

class Monitor
{
 public:
  Monitor();
  ~Monitor();

  void Run();
  
 protected:
  Watcher watcher;
  
  TCanvas* canvas;

  bool chk_ch_occupied[NUM_CHANNEL];
  Painter* painter[NUM_CHANNEL];
};

#endif /* __Monitor_h__ */
