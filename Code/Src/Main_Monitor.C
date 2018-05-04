#include <iostream>
#include <chrono>
#include <thread>

#include "Monitor.h"
#include "Watcher.h"

#define MAXLINE 512

using namespace std;

int main()
{
  char* path = getenv("QC_SW_PATH");
  if(path==NULL)
    {
      cerr << "Enviromental variables aren't set. Source Env.sh first." << endl;
      return -1;
    }
  
  Monitor monitor;
  monitor.Run();
  
   return 0;
}
