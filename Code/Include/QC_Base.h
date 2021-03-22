#ifndef __QC_Base_h__
#define __QC_Base_h__

#include <vector>
#include <chrono>
#include <thread>
#include <bitset>

#include "Client.h"

#define IMON "IMonL"

using namespace std;
using namespace chrono;

class QC_Base
{
public:
  QC_Base(const string& a_foil_name, const int& a_trial_number, const int& a_channel, const float& a_rh, const float& a_temp, const string& a_tester, const string& a_path, const bool& a_verbosity=false);
  ~QC_Base();
  
  typedef struct _config_data
  {
    float time;
    float vset;
  } Config_Data;
  
  virtual void Run() = 0;
  
protected:
  string type;
  string foil_name;
  int trial_number;
  int channel;
  float rh;
  float temp;
  string tester;
  string path;
  
  int n_trip_total;
  int n_trip_stage;
  int* n_trial;
  
  string path_result;
  ofstream result_out;
  
  Client client;
  
  vector<Config_Data> vec_config_data;

  bool mode_imonrange;
  
  void Check_Stability(bool& stability, bool& reset);
  bool Check_Trip();
  void Finalization(const string& HV_status="OFF");
  float Get_VSet(const int& index);
  virtual void Initialization();
  virtual void Initialization_HV() = 0;
  bool Recover_Trip(const float& vset, const system_clock::time_point& process_start, float& recovery_duration, const bool& mode_pull_back=false);
  void Read_Config_Data();
  void Result_Log_Maker();
  void Set_IMonRange(const string& mode);
  
  virtual void Body() = 0;
};

#endif /* __QC_Base_h__ */
