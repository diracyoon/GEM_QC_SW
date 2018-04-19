#ifndef __QC_Base_h__
#define __QC_Base_h__

#include <vector>
#include <chrono>
#include <thread>

#include "Client.h"

using namespace std;
using namespace chrono;

class QC_Base
{
 public:
  QC_Base(const int& a_runnumber, const string& a_foil_name, const int& a_channel, const float& a_rh, const float& a_temp, const string& a_tester, const string& a_path, const bool& a_verbosity=false);
  ~QC_Base();

    typedef struct _config_data
    {
      float time;
      float vset;
    } Config_Data;

    virtual void Run() = 0;
    
 protected:
    int runnumber;
    string foil_name;
    float rh;
    float temp;
    string tester;
    string path;
    
    int n_trip;

    string path_result;
    ofstream result_out;
      
    Client client;

    vector<Config_Data> vec_config_data;

    void Finalization();
    void Initialization(const string& type);
    void Initialization_HV();
    void Read_Config_Data(const string& config_file);
    float Recover_Trip(const float& vset);
    virtual void Body() = 0;
    void Result_Log_Maker(const string& type);
    float Scan_Config(const float& time_run);
};

#endif /* __QC_Base_h__ */
