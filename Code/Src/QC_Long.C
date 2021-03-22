#include "QC_Long.h"

//////////

QC_Long::QC_Long(const string& a_foil_name, const int& a_trial_number, const int& a_channel, const float& a_rh, const float& a_temp, const string& a_tester, const string& a_path, const bool& a_verbosity) : QC_Base(a_foil_name, a_trial_number, a_channel, a_rh, a_temp, a_tester, a_path, a_verbosity)
{
  type = "QC_Long";
}//QC_Long::QC_Long()


//////////

QC_Long::~QC_Long()
{
}//QC_Long::~QC_Long()

//////////

void QC_Long::Run()
{
  Initialization();
  Body();
  Finalization();
  
  return;
}//void QC_Long::Run()

//////////

void QC_Long::Body()
{
  cout << "QC_Long: Start QC_Long test!!" << endl;

  system_clock::time_point process_start = system_clock::now();

  bool stability_flipflop = false;
  for(int i=0; i<vec_config_data.size(); i++)
    {
      system_clock::time_point stage_start = system_clock::now();

      //To avoid the OVV issue
      Set_IMonRange("HIGH");

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

		  cout << "QC_Logg: Ch# = " << channel << ": Too many trips at this stage V_Set = " << vset << ". Abort QC." << endl;
		  throw "Too many trip";
		}

	      //obsolete
	      trip_duration += recovery_duration;

	      //reset stage duration
	      stage_start = system_clock::now();
	    }

	  //check HV stability. If HV is on ramping up or down, don't count time.
	  bool stability;
	  bool reset;
	  Check_Stability(stability, reset);

	  //unstable
	  if(stability==false)
	    {
	      if(stability_flipflop==true)
		{
		  stability_flipflop=false;
		  Set_IMonRange("HIGH");
		}

	      //reset stage duration
	      if(reset==true) stage_start = system_clock::now();
	    }
	  else if(stability==true)
	    {
	      if(stability_flipflop==false)
		{
		  stability_flipflop = true;
		  Set_IMonRange("LOW");
		}
	    }
	  
	  float vmon = client.Request_HV_Control_Get("VMon");
	  float imon;
	  if(mode_imonrange==false) imon = client.Request_HV_Control_Get("IMonH"); 
	  else if(mode_imonrange==true) imon = client.Request_HV_Control_Get("IMonL");

	  //run time
	  system_clock::time_point time_current = system_clock::now();
	  
	  duration<float> process_duration = time_current - process_start;
	  duration<float> stage_duration = time_current - stage_start;

	  float process_duration_f = process_duration.count();
	  float stage_net_duration_f = stage_duration.count();
	  //float stage_net_duration_f = stage_duration_f - trip_duration;
	  	  
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

void QC_Long::Initialization_HV()
{
  client.Request_HV_Control_Set("Pw", 0);
  client.Request_HV_Control_Set("Pw", 1);
  
  client.Request_HV_Control_Set("MaxV", 605);
  
  client.Request_HV_Control_Set("RUp", 5);
  client.Request_HV_Control_Set("RDwn", 10);
  
  //client.Request_HV_Control_Set("ImonRange", 1);//1 means low
  client.Request_HV_Control_Set("ImonRange", 0);//0 means high
  client.Request_HV_Control_Set("ISet", 2);

  client.Request_HV_Control_Set("Trip", 0.0);
  client.Request_HV_Control_Set("PDwn", 0);

  n_trip_total = 0;
  n_trip_stage = 0;
}//void QC_Long::Initialization_HV()

//////////
