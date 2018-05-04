#ifndef __Watcher_h__
#define __Watcher_h__

#include <iostream>
#include <sstream>
#include <cstdlib>

#define MAXLINE 512 

using namespace std;

typedef struct _watch_result
{
  int pid;
  string process;
  int runnumber;
  string foil_name;
  int channel;
  float rh;
  float temperature;
  string tester;  
} Watch_Result;

class Watcher
{
 public:
  Watcher();
  ~Watcher();

  int Watch_Process(Watch_Result watch_result[]);
  
 protected:
  string qc_sw_path;
  
  FILE *fp;
};

#endif /* __Watcher_h__ */
