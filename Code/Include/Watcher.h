#ifndef __Watcher_h__
#define __Watcher_h__

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>

#define MAXLINE 512 

using namespace std;

typedef struct _watch_result
{
  int pid;
  string process;
  string foil_name;
  int trial_number;
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

  void Watch_Process(vector<Watch_Result>& vec_watch_result);
  
 protected:
  string qc_sw_path;
  
  FILE *fp;
};

#endif /* __Watcher_h__ */
