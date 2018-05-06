#include <iostream>
#include <chrono>
#include <thread>

#include <TROOT.h>
#include <TApplication.h>

#include "Monitor.h"
#include "Watcher.h"

#define MAXLINE 512

using namespace std;

extern void InitGui();
VoidFuncPtr_t initfuncs[] = {InitGui, 0};
TROOT root("Monitor","Monitor", initfuncs);
TApplication* theApp;

int main(int argc, char** argv)
{
  char* path = getenv("QC_SW_PATH");
  if(path==NULL)
    {
      cerr << "Enviromental variables aren't set. Source Env.sh first." << endl;
      return -1;
     }
  
  theApp = new TApplication("Monitor", &argc, argv);
  
  Monitor monitor;
  monitor.Run();
  
  theApp->Run(true);
  
   return 0;
}
