#include "Watcher.h"

//////////

Watcher::Watcher()
{
  char* path = getenv("QC_SW_PATH");
  qc_sw_path = path;
}//Watcher::Watcher()

//////////

Watcher::~Watcher()
{
}//Watcher::~Watcher()

//////////

void Watcher::Watch_Process(vector<Watch_Result>& vec_watch_result)
{
  fp = popen("ps -au", "r");
  if(fp==NULL) throw "class Watcher: Can not open popen!";
  
  char buf[MAXLINE];
  while(fgets(buf, MAXLINE, fp)!=NULL)
    {
      string temp = buf;
      
      istringstream iss(temp);

      int pid;
      string cmd;
      
      iss >> temp;
      iss >> pid;
      iss >> temp;
      iss >> temp;
      iss >> temp;
      iss >> temp;
      iss >> temp;
      iss >> temp;
      iss >> temp;
      iss >> temp;
      getline(iss, cmd);
      
      
      if(cmd.find(qc_sw_path+"/Bin/QC_Long")!=string::npos || cmd.find(qc_sw_path+"/Bin/Preparation_QC_Long")!=string::npos)
	{	  
	  Watch_Result watch_result;
	  
	  watch_result.pid = pid;

	  iss.str(cmd);
	  iss >> watch_result.process;
	  iss >> watch_result.runnumber;
	  iss >> watch_result.foil_name;
	  iss >> watch_result.channel;
	  iss >> watch_result.rh;
	  iss >> watch_result.temperature;
	  iss >> watch_result.tester;

	  vec_watch_result.push_back(watch_result);
	}
    }

  pclose(fp);
  
  return;
}//int Watcher::Watch_Process()

//////////
