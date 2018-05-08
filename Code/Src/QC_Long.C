#include "QC_Long.h"

//////////

QC_Long::QC_Long(const int& a_runnumber, const string& a_foil_name, const int& a_channel, const float& a_rh, const float& a_temp, const string& a_tester, const string& a_path, const bool& a_verbosity) : QC_Base(a_runnumber, a_foil_name, a_channel, a_rh, a_temp, a_tester, a_path, a_verbosity)
{
}//QC_Long::QC_Long()


//////////

QC_Long::~QC_Long()
{
}//QC_Long::~QC_Long()

//////////

void QC_Long::Run()
{
  Initialization("QC_Long");
  Body();
  Finalization();
  
  return;
}//void QC_Long::Run()

//////////

void QC_Long::Body()
{
  cout << "Start QC_Long test!!" << endl;

  system_clock::time_point process_start = system_clock::now();

  for(int i=0; i<vec_config_data.size(); i++)
    {
      system_clock::time_point stage_start = system_clock::now();

      float vset = Get_VSet(i);
      client.Request_HV_Control_Set("VSet", vset);
      
      float trip_duration = 0;
      n_trip_stage = 0;
      
      while(1)
	{
	  //trip check
	  if(Check_Trip()==true)
	    {
	      float recovery_duration;
	      bool turn_off = Recover_Trip(vset, process_start, recovery_duration, true);

	      //if trip ocurrs more than three times, abort QC
	      if(turn_off==true)
		{
		  //turn off HV for safety
		  client.Request_HV_Control_Set("Pw", 0);

		  cout << "class QC_Log, Ch# = " << channel << ": Too many trips at this stage V_Set = " << vset << ". Abort QC." << endl;
		  throw "Too many trip";
		}
	      
	      trip_duration += recovery_duration;
	    }

	  //check HV stability. If HV is on ramping up or down, don't count time.
	  if(Check_Stability()==false) stage_start = system_clock::now();
	    
	  float vmon = client.Request_HV_Control_Get("VMon");
	  float imon = client.Request_HV_Control_Get(IMON);
	  
	  //run time
	  system_clock::time_point time_current = system_clock::now();
	  
	  duration<float> process_duration = time_current - process_start;
	  duration<float> stage_duration = time_current - stage_start;

	  float process_duration_f = process_duration.count();
	  float stage_duration_f = stage_duration.count();
	  float stage_net_duration_f = stage_duration_f - trip_duration;
	  	  
	  result_out << process_duration_f << " " << " " << vset << " " << vmon << " " << imon << endl;
	  
	  if((int)process_duration_f%10==0) cout << "Process duration = " << process_duration_f << "s, stage duration = " << stage_net_duration_f << "s, V_set = " << vset << "V, V_Mon = " << vmon << "V, I_Mon = " << imon << "uA" << endl;

	  //end of stage
	  if(vec_config_data[i].time<stage_net_duration_f) break;
	  
	  this_thread::sleep_for(chrono::seconds(1));
	}//while
    }//for
  
  return;
}//void QC_Long::Body()

//////////
