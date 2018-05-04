#include "Preparation_QC_Long.h"

//////////

Preparation_QC_Long::Preparation_QC_Long(const int& a_runnumber, const string& a_foil_name, const int& a_channel, const float& a_rh, const float& a_temp, const string& a_tester, const string& a_path, const bool& a_verbosity) : QC_Base(a_runnumber, a_foil_name, a_channel, a_rh, a_temp, a_tester, a_path, a_verbosity)
{
}//Preparation_QC_Long::Preparation_QC_Long()

//////////

Preparation_QC_Long::~Preparation_QC_Long()
{
}//Preparation_QC_Long::~Preparation_QC_Long()

//////////

void Preparation_QC_Long::Run()
{
  Initialization("Preparation_QC_Long");
  Body();
  Finalization("ON");
  
  return;
}//void Preparation_QC_Long::Run()

//////////

void Preparation_QC_Long::Body()
{  
  cout << "Start Preparation QC_Long test!!" << endl;

  system_clock::time_point process_start = system_clock::now();
  
  for(int i=0; i<vec_config_data.size(); i++)
    {
      system_clock::time_point stage_start = system_clock::now();
      
      float vset = Get_VSet(i);
      client.Request_HV_Control_Set("VSet", vset);
      
      float trip_duration = 0;
      n_trip_stage = 0;

      float imon_old = 0;
      float imon = 0;
      
      while(1)
	{
	  //trip check
	  if(Check_Trip()==true)
	    {
	      float recovery_duration;
	      bool pull_back = Recover_Trip(vset, process_start, recovery_duration, true);
	      
	      if(pull_back==true)
		{
		  cout << "class Preparation_QC_Log, Ch# = " << channel << ": Too many trips. Pull back." << endl;
		  
		  n_trial[i]++;
		  
		  if(3<n_trial[i])
		    {
		      //turn off HV for safety
		      client.Request_HV_Control_Set("Pw", 0);
		      
		      cout << "class Preparation_QC_Log, Ch# = " << channel << ": Too many trials at this stage V_Set = " << vset << ". Abort QC." << endl;
		      throw "Too many trials";
		    }
		  
		  //pull back. It's a bit risky trick but quick
		  i = i - 2;
		  if(i<0) i = 0;
		  
		  break;
		}
	      //reset stage start time
	      stage_start = system_clock::now();
	    }
	  
	  imon_old = imon;
	  
	  float vmon = client.Request_HV_Control_Get("VMon");
	  imon = client.Request_HV_Control_Get(IMON);

	  //current stability check. if current is not stable within 20%, reset stage start time  
	  if(0.2<fabs(imon_old-imon)/imon) stage_start = system_clock::now();
	  	  
	  //run time
	  system_clock::time_point time_current = system_clock::now();
	  
	  duration<float> process_duration = time_current - process_start;
	  duration<float> stability_duration = time_current - stage_start;

	  float process_duration_f = process_duration.count();
	  float stability_duration_f = stability_duration.count();
	  
	  result_out << process_duration_f << " " << vset << " " << vmon << " " << imon << endl;

	  cout << fixed;
	  if((int)process_duration_f%10==0) cout << "Process duration = " << setprecision(2) << process_duration_f << "s, stability duration = " << setprecision(2) << stability_duration_f << "s, V_set = " << setprecision(2) << vset << "V, V_Mon = " << setprecision(2) << vmon << "V, I_Mon = " << setprecision(2) << imon << "uA" << endl;

	  //end of stage
	  if(vec_config_data[i].time<stability_duration_f) break;

	  this_thread::sleep_for(chrono::seconds(1));
	}//while
    }//

  return;
}//void Preparation_QC_Long::Body()

//////////
