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

  float i_history[5] = {0};
  
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
	  if(client.Request_HV_Control_Get("Pw")==false)
	    {
	      n_trip_total++;
	      n_trip_stage++;

	      cout << "class Preparation_QC_Log, Ch# = " << channel << ": Trip!! # of trip in the stage= " << n_trip_stage << ", # of trip in whole run = " << n_trip_total << endl;

	      //pull back
	      if(3<n_trip_stage)
		{
		  cout << "class Preparation_QC_Log, Ch# = " << channel << ": Too many trip. Pull back." << endl;
		  i =- 2;
		 		  
		  break;
		}
	      
	      Recover_Trip(vset, process_start);

	      //reset stage start time
	      stage_start = system_clock::now();
	    }
	  
	  float vmon = client.Request_HV_Control_Get("VMon");
	  float imon = client.Request_HV_Control_Get("IMonH");

	  //current stability check
	  for(int j=0; j<4; j++) i_history[j] = i_history[j+1];
	  i_history[4] = imon;

	  
	  
	  //run time
	  system_clock::time_point time_current = system_clock::now();
	  
	  duration<float> process_duration = time_current - process_start;
	  duration<float> stability_duration = time_current - stage_start;

	  float process_duration_f = process_duration.count();
	  float stability_duration_f = stability_duration.count();
	  
	  result_out << process_duration_f << " " << vset << " " << vmon << " " << imon << endl;

	  if((int)process_duration_f%10==0) cout << "Process duration = " << process_duration_f << "s, stability duration = " << stability_duration_f << "s, V_set = " << vset << "V, V_Mon = " << vmon << "V, I_Mon = " << imon << "uA" << endl;

	  //end of stage
	  if(vec_config_data[i].time<stability_duration_f) break;

	  this_thread::sleep_for(chrono::seconds(1));
	}//while
    }//

  return;
}//void Preparation_QC_Long::Body()

//////////
