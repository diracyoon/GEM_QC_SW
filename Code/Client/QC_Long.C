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

  float vset_old = -999;
  float trip_duration = 0;

  system_clock::time_point time_start = system_clock::now();

  while(1)
    {
      //run time
      system_clock::time_point time_current = system_clock::now();

      duration<float> duration = time_current - time_start;

      float process_duration = duration.count();
      float run_duration = process_duration - trip_duration;

      float vset = Scan_Config(run_duration);

      //end of QC
      if(vset<0) break;
      else if(vset!=vset_old)
	{
	  client.Request_HV_Control_Set("VSet", vset);
	  vset_old = vset;
	}

      if(client.Request_HV_Control_Get("Pw")==false) trip_duration += Recover_Trip(vset);

      float vmon = client.Request_HV_Control_Get("VMon");
      float imon = client.Request_HV_Control_Get("IMonH");

      result_out << run_duration << " " << vset << " " << vmon << " " << imon << endl;

      if((int)run_duration%10==0) cout << "Run duration = " << run_duration << "s, V_set = " << vset << "V, V_Mon = " << vmon << "V, I_Mon = " << imon << "uA" << endl;

      this_thread::sleep_for(chrono::seconds(1));
    } 
  
  return;
}//void QC_Long::Body()

//////////
