#include "Monitor.h"

//////////

Monitor::Monitor() : watcher()
{
  for(int i=0; i<NUM_CHANNEL; i++) chk_ch_occupied[i] = false;
}//Monitor::Monitor()

//////////

Monitor::~Monitor()
{
}//Monitor::~Monitor()

//////////

void Monitor::Run()
{
  canvas = new TCanvas("Monitor", "Monitor", 1200, 800);
  canvas->Divide(4, 2);
  canvas->Draw();
    
  while(1)
    {
      vector<Watch_Result> vec_watch_result;
      watcher.Watch_Process(vec_watch_result);
      
      cout << "Number of occupied channel = " << vec_watch_result.size() << endl;
      
      for(int i=0; i<NUM_CHANNEL; i++)
	{
	  //search channel in vec_watch_result
	  bool chk_ch_found = false;
	  int index = -999;
	  for(int j=0; j<vec_watch_result.size(); j++)
	    {
	      if(i==vec_watch_result[j].channel)
		{
		  chk_ch_found = true;
		  index = j;
		  break;
		}
	    }
	  
	  if(chk_ch_found==true)
	    {
	      //new measurement 
	      if(chk_ch_occupied[i]==false)
		{
		  watch_result[i] = vec_watch_result[index];
		  
		  cout << "New QC detected. Occupied channel = " << watch_result[i].channel << ", PID = " << watch_result[i].pid << ", Process = " << watch_result[i].process << endl;
		  this_thread::sleep_for(chrono::seconds(1));
		  
		  chk_ch_occupied[i] = true;

		  string process = watch_result[i].process.substr(watch_result[i].process.find_last_of("/")+1);
		  		  
		  string file_path = getenv("QC_SW_PATH");
		  file_path +=  "/Output/" + process + "/" + watch_result[i].foil_name + "/" + watch_result[i].foil_name + "_"  + string(2 - to_string(watch_result[i].trial_number).length(), '0') + to_string(watch_result[i].trial_number) + ".result";
		 
		  painter[i] = new Painter(file_path);
		}
	      //ongoing measurement
	      else
		{
		  painter[i]->Update();
		  painter[i]->Draw(canvas->cd(i+1));
		}
	    }//channel occupied
	  else
	    {
	      //end measurement
	      if(chk_ch_occupied[i]==true)
		{
		  delete painter[i];

		  //submit to logbook
		  string foil_name = watch_result[i].foil_name;
		  string process = watch_result[i].process;
		  int trial_number = watch_result[i].trial_number;
		  
		  Submitter submitter(foil_name, process, trial_number);
		  
		  chk_ch_occupied[i] = false;  
		 		  
		  cout << "Channel #" << i << " is released." << endl;
		}
	    }//channel released
	}//channel loop
	
      gSystem->ProcessEvents();
      this_thread::sleep_for(chrono::seconds(5));
    }

  return;
}//void Monitor::Run()

//////////
