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

int Watcher::Watch_Process(Watch_Result watch_result[])
{
  fp = popen("ps -au", "r");
  if(fp==NULL) throw "class Watcher: Can not open popen!";
  
  int num_occupied_channel = 0;
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
	  iss.str(cmd);
	  
	  watch_result[num_occupied_channel].pid = pid;
	  iss >> watch_result[num_occupied_channel].process;
	  iss >> watch_result[num_occupied_channel].runnumber;
	  iss >> watch_result[num_occupied_channel].foil_name;
	  iss >> watch_result[num_occupied_channel].channel;
	  iss >> watch_result[num_occupied_channel].rh;
	  iss >> watch_result[num_occupied_channel].temperature;
	  iss >> watch_result[num_occupied_channel].tester;

	 
	  
	  num_occupied_channel++;
	}
    }

  pclose(fp);
  
  return num_occupied_channel;
}//int Watcher::Watch_Process()

//////////
