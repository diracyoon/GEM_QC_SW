#include "Monitor.h"

//////////

Monitor::Monitor() : watcher(), canvas("canvas", "canvas", 800, 500)
{
 
}//Monitor::Monitor()

//////////

Monitor::~Monitor()
{
}//Monitor::~Monitor()

//////////

void Monitor::Run()
{
  //canvas.Draw();
  //canvas.Divide(2, 2);

  
  while(1)
    {
      Watch_Result watch_result[4]; 
      int num_occupied_channel = watcher.Watch_Process(watch_result);

      cout << "Number of occupied channel = " << num_occupied_channel << endl;

      for(int i=0; i<num_occupied_channel; i++)
	{
	  cout << "Occupied channel = " << watch_result[i].channel << ", PID = " << watch_result[i].pid << ", Process = " << watch_result[i].process << endl; 
	}
      
      this_thread::sleep_for(chrono::seconds(5));
    }

  return;
}//void Monitor::Run()

//////////
