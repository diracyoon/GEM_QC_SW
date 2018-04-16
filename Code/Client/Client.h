#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>
#include <vector>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MSG_SIZE 64

using namespace std;
using namespace chrono;

class Client
{
 public:
  Client(const string& a_path_fifo_server, const int& a_channel);
  ~Client();

  void Run();

  typedef struct _config_data
  {
    int time;
    float vset;
  } Config_Data;
  
 protected:
  int channel;

  string path_fifo_client;
  string path_fifo_server;
  
  int fd_client;
  int fd_server;

  vector<Config_Data> vec_config_data;
  
  void Connect_To_Server();
  void Disconnect_From_Server();
  void Initialization();
  void Initialization_QC();
  string Receive_From_Server();
  void Read_Config_Data();
  bool Request_HV_Control_Get(const string& parameter, float& value);
  bool Request_HV_Control_Set(const string& parameter, const float& value);
  void Transmit_To_Server(const string& msg);
  void QC_Long();
  void QC_Long_Body();
};
