#include "QC_Base.h"

//////////

QC_Base::QC_Base(const string& a_foil_name, const int& a_trial_number, const int& a_channel, const float& a_rh, const float& a_temp, const string& a_tester, const string& a_path, const bool& a_verbosity) : foil_name(a_foil_name), trial_number(a_trial_number), channel(a_channel), rh(a_rh), temp(a_temp), tester(a_tester), path(a_path), client(a_channel, a_path, a_verbosity)
{
  mode_imonrange= false;
}//QC_Base::QC_Base()

//////////

QC_Base::~QC_Base()
{
  delete[] n_trial;
}//QC_Base::~QC_Base()

//////////

void QC_Base::Check_Stability(bool& stability, bool& reset)
{
  int status = client.Request_HV_Control_Status();

  bool ramping_up = status & 0x2;
  bool ramping_down = status & 0x4;
  bool ovv = status & 0x10;
  bool udv = status & 0x20;
  bool trip = status & 0x80;
  
  if(ramping_up||ramping_down||ovv||udv||trip) stability = false;
  else stability = true;

  if(ramping_up||ramping_down||trip) reset = true;
  else reset = false;

  return;
}//void QC_Base::Check_Stability(bool& stability, bool& reset)

//////////

bool QC_Base::Check_Trip()
{
  int status = client.Request_HV_Control_Status();
  
  bool trip = status & 0x80;
  
  return trip;
}//bool QC_Base::Check_Trip()

//////////

void QC_Base::Finalization(const string& HV_status)
{
  if(HV_status.find("OFF")!=string::npos) client.Request_HV_Control_Set("Pw", 0);
     
  result_out << "##############################" << endl;
  result_out << "# of trip = " << n_trip_total << endl;
  time_t time = system_clock::to_time_t(system_clock::now());
  result_out << "End time: " << ctime(&time);

  cout << "QC_Base: QC done." << endl;

  return;
}//void QC_Base::QC_Long_Finalization()

//////////

float QC_Base::Get_VSet(const int& index)
{
  if(index<vec_config_data.size()) return vec_config_data[index].vset;
  else return -1;
}//float QC_Base::Get_VSet(const int& index)

//////////

void QC_Base::Initialization()
{
  Result_Log_Maker();
  Read_Config_Data();

  Initialization_HV();

  return;
}//void QC_Base::Initialization(const string& type)

//////////

bool QC_Base::Recover_Trip(const float& vset, const system_clock::time_point& process_start, float& recovery_duration, const bool& mode_pull_back)
{
  system_clock::time_point time_start = system_clock::now();

  n_trip_total++;
  n_trip_stage++;
  
  cout << "QC_Base, Ch# = " << channel << ": Trip!! number of trip in the stage = " << n_trip_stage << ", number of trip in whole run = " << n_trip_total << endl;
  cout << "QC_Base: Start recovery process." << endl;

  //pull back
  if(mode_pull_back==true && 3<n_trip_stage) return true;

  //set ImonRange=High to prevent overvoltage
  if(type.compare("QC_Long")==0) Set_IMonRange("HIGH");
  
  ///trip recover
  client.Request_HV_Control_Set("Pw", 0);
  this_thread::sleep_for(chrono::seconds(5));

  client.Request_HV_Control_Set("Pw", 1);
  this_thread::sleep_for(chrono::seconds(5));
  
  client.Request_HV_Control_Set("VSet", vset);
  this_thread::sleep_for(chrono::seconds(5));
  
  //voltage recover
  while(1)
    {
      //trip check
      if(Check_Trip()==true) return Recover_Trip(vset, process_start, recovery_duration, mode_pull_back);

      //voltage stabilization condition
      bool stability;
      bool reset;
      Check_Stability(stability, reset);

      if(stability==true)
	{
	  cout << "QC_Base: Recovery process done." << endl;
	  break;
	}
      
      //run time
      system_clock::time_point time_current = system_clock::now();

      duration<float> process_duration = time_current - process_start;
      float process_duration_f = process_duration.count();
      
      float vmon = client.Request_HV_Control_Get("VMon");
      float imon = client.Request_HV_Control_Get("IMonH");

      result_out << process_duration_f << " " << vset << " " << vmon << " " << imon << endl;
      
      cout << "QC_Base: Recovering trip. VSet: " << vset << ", VMon = " << vmon << endl;
            
      this_thread::sleep_for(chrono::milliseconds(500));
    }
  
  system_clock::time_point time_end = system_clock::now();
  
  duration<float> duration = time_end - time_start;
  recovery_duration = duration.count();

  //return to ImonRange=Low  
  if(type.compare("QC_Long")==0) Set_IMonRange("LOW");                                                                 

  return false;
}//float QC_Base::Recover_Trip(const float& vset)
  
//////////

void QC_Base::Read_Config_Data()
{
  cout << "QC_Base: Read Config. data." << endl;

  string config_file = type + ".config";
  string config_path = path + "/Config/" + config_file;
  
  ifstream fin_config;
  fin_config.open(config_path);
  if(!fin_config.is_open()) throw "class QC_Base: Can not find the config file.";

  result_out << "Read config file " << config_file << endl;
  
  string buf;
  while(!fin_config.eof())
    {
      getline(fin_config, buf);
      if(buf.empty()) break;

      istringstream iss(buf);

      Config_Data config_data;

      iss >> config_data.time;
      iss >> config_data.vset;

      vec_config_data.push_back(config_data);

      result_out << config_data.time << " " << config_data.vset << endl;
    }
  result_out << "##############################" << endl;

  n_trial = new int[vec_config_data.size()];
  for(int i=0; i<vec_config_data.size(); i++) n_trial[i] = 0;
  
  return;
}//void Client::Read_Config_Data()

//////////

void QC_Base::Result_Log_Maker()
{
  path_result = path;
  path_result += "/Output/" + type + "/" + foil_name + "/" + foil_name + "_";
  path_result += string(2 - to_string(trial_number).length(), '0') + to_string(trial_number);
  path_result += ".result";
  
  //check file existence
  ifstream fin_check;
  fin_check.open(path_result);
  if(fin_check.is_open()) throw "class QC_Base: Can not open file for result log. Maybe runnumber is conflicting.";

  result_out.open(path_result);
  if(!result_out.is_open()) throw "class QC_Base: Can not open file for result log.";

  result_out << "Mode: " << client.Get_Mode() << endl;
  result_out << "Foil name: " << foil_name << endl;
  result_out << "Trial number: " << trial_number << endl;
  result_out << "Channel: " << channel << endl;
  result_out << "Relative humidity (%): " << rh  << endl;
  result_out << "Temperature (c): " << temp << endl;
  result_out << "Tester: " << tester << endl;

  time_t time = system_clock::to_time_t(system_clock::now());
  result_out << "Start time: " << ctime(&time);
  result_out << "##############################" << endl;

  return;
}//void QC_Base::Result_Log_Maker(const string& type)

//////////

void QC_Base::Set_IMonRange(const string& mode)
{
  if(mode.compare("LOW")==0)
    {
      cout << "Change ImonRange to Low." << endl;

      mode_imonrange = true;
      client.Request_HV_Control_Set("ImonRange", 1);
    }
  else if(mode.compare("HIGH")==0)
    {
      cout << "Change ImonRange to High." << endl;

      mode_imonrange = false;
      client.Request_HV_Control_Set("ImonRange", 0);
    }
  else cout << "Set_IMonRange: Wrong argument." << endl;
  
  this_thread::sleep_for(chrono::seconds(1));
  
  return;
}//void QC_Base::Set_IMonRange(const string& mode)

//////////
