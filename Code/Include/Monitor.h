#ifndef __Monitor_h__
#define __Monitor_h__

#include <chrono>
#include <thread>

#include <TCanvas.h>

#include "Watcher.h"

using namespace std;

class Monitor
{
 public:
  Monitor();
  ~Monitor();

  void Run();
  
 protected:
  Watcher watcher;
  
  TCanvas canvas;
  
};

#endif /* __Monitor_h__ */
