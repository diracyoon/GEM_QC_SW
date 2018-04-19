#include "QC_Base.h"

//////////

QC_Base::QC_Base(const int& a_runnumber, const string& a_foil_name, const int& a_channel, const float& a_rh, const float& a_temp, const string& a_tester, const string& a_path, const bool& a_verbosity) : runnumber(a_runnumber), foil_name(a_foil_name), rh(a_rh), temp(a_temp), tester(a_tester), path(a_path), client(a_channel, a_path, a_verbosity)
{
}//QC_Base::QC_Base()

//////////

QC_Base::~QC_Base()
{
}//QC_Base::~QC_Base()

//////////

void QC_Base::Finalization()
{
  client.Request_HV_Control_Set("Pw", 0);

  result_out << "##############################" << endl;
  result_out << "# of trip = " << n_trip << endl;
  time_t time = system_clock::to_time_t(system_clock::now());
  result_out << "End time: " << ctime(&time);

  cout << "QC done." << endl;

  return;
}//void QC_Base::QC_Long_Finalization()

//////////

void QC_Base::Initialization(const string& type)
{
  Result_Log_Maker(type);
  Read_Config_Data(type+".config");

  Initialization_HV();

  return;
}//void QC_Base::Initialization(const string& type)

//////////

void QC_Base::Initialization_HV()
{
  client.Request_HV_Control_Set("Pw", 1);

  client.Request_HV_Control_Set("MaxV", 615);
  client.Request_HV_Control_Set("RUp", 1);
  client.Request_HV_Control_Set("RDwn", 50);

  client.Request_HV_Control_Set("ISet", 1);
  client.Request_HV_Control_Set("Trip", 0.1);
  client.Request_HV_Control_Set("PDwn", 0);

  n_trip = 0;
  
  return;
}//void QC_Base::Initialization_HV()

//////////

void QC_Base::Read_Config_Data(const string& config_file)
{
  cout << "Read Config. data." << endl;

  string config_path = path + "/Config/" + config_file;
  
  ifstream fin_config;
  fin_config.open(config_path);
  if(!fin_config.is_open()) throw "class Client: Can not find the config file.";

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

  return;
}//void Client::Read_Config_Data()

//////////

float QC_Base::Recover_Trip(const float& vset)
{
  system_clock::time_point time_start = system_clock::now();

  n_trip++;

  cout << "class QC_Base: Trip!! # of trip = " << n_trip << " Start recovery process." << endl;

  ///trip recover
  client.Request_HV_Control_Set("Pw", 0);
  client.Request_HV_Control_Set("Pw", 1);
  client.Request_HV_Control_Set("VSet", vset);

  //voltage recover
  float vmon_old = -1;
  while(1)
    {
      float vmon = client.Request_HV_Control_Get("VMon");

      cout << "Recovering trip. VSet: " << vset << ", VMon = " << vmon << endl;

      //voltage stabilization condition
      if(fabs(vmon-vset)/vset<0.05 && (vmon-vmon_old)<0.2)
	{
	  cout << "Recovery process done." << endl;
	  break;
	}

      vmon_old = vmon;

      this_thread::sleep_for(chrono::seconds(1));
    }

  system_clock::time_point time_end = system_clock::now();

  duration<float> duration = time_end - time_start;
  float process_duration = duration.count();

  return process_duration;
}//void QC_Base::Recover_Trip(const float& vset)

//////////

void QC_Base::Result_Log_Maker(const string& type)
{
  path_result = path + "/Output/" + type + "/" + to_string(runnumber) + "_" + foil_name + ".result";

  //check file existence
  ifstream fin_check;
  fin_check.open(path_result);
  if(fin_check.is_open()) throw "class QC_Base: Can not open file for result log. Maybe runnumber is conflicting.";

  result_out.open(path_result);
  if(!result_out.is_open()) throw "class QC_Base: Can not open file for result log.";

  result_out << "Runnumber: " << runnumber << endl;
  result_out << "Foil name: " << foil_name << endl;
  result_out << "Channel: " << runnumber << endl;
  result_out << "Relative humidity: " << rh << "%" << endl;
  result_out << "Temperature: " << temp << "C" << endl;
  result_out << "Tester: " << tester << endl;

  time_t time = system_clock::to_time_t(system_clock::now());
  result_out << "Start time: " << ctime(&time);
  result_out << "##############################" << endl;

  return;
}

//////////

float QC_Base::Scan_Config(const float& time_run)
{
  for(int i=0; i<vec_config_data.size(); i++)
    {
      if(vec_config_data[i].time<time_run && time_run<vec_config_data[i+1].time) return vec_config_data[i].vset;
    }

  return -1;
}//float QC_Base::Scan_Config(const float& time_run)

//////////
