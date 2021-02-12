#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>

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
      cout << "Enviromental variables aren't set. Source Env.sh first." << endl;
      return -1;
     }

  if(argc!=1&&argc!=2)
    {
      cout << "Wrong number of argc." << endl;
      cout << "argc[1]: submit to e-log or not i.e. TRUE or FALSE. Default is TRUE." << endl;
      exit(1);
    }

   bool chk_submit = true;
  if(argc==2)
    {
       if(strcmp(argv[1], "FALSE")==0) chk_submit = false;
    }
   
  theApp = new TApplication("Monitor", &argc, argv);
  
  Monitor monitor(chk_submit);
  monitor.Run();
  
  theApp->Run(true);
  
   return 0;
}
