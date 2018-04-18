#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>
#include <vector>
#include <cmath>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MSG_SIZE 64

using namespace std;
using namespace chrono;

class Client
{
 public:
  Client(const int& a_runnumber, const string& a_foil_name, const int& a_channel, const string& a_tester, const string& a_path, const bool& a_verbosity=false);
  ~Client();

  void Run();

  typedef struct _config_data
  {
    float time;
    float vset;
  } Config_Data;
  
 protected:
  int runnumber;
  string foil_name;
  int channel;
  string tester;

  bool verbosity;

  string path;
  string path_fifo_client;
  string path_fifo_server;
  
  int fd_client;
  int fd_server;
  
  vector<Config_Data> vec_config_data;

  string path_result;
  ofstream result_out;

  int n_trip;
  
  void Connect_To_Server();
  void Disconnect_From_Server();
  void Initialization();
  void Initialization_QC();
  string Receive_From_Server();
  void Read_Config_Data();
  float Recover_Trip(const float& vset);
  void Result_Log_Maker();
  float Request_HV_Control_Get(const string& parameter);
  void Request_HV_Control_Set(const string& parameter, const float& value);
  float Scan_Config(const float& time_run);
  void Transmit_To_Server(const string& msg);
  void QC_Long();
  void QC_Long_Body();
  void QC_Long_Finalization();
  void QC_Long_Initialization();
};
