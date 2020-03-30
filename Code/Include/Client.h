#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>
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
  Client(const int& a_channel, const string& a_path, const bool& a_verbosity=false);
  ~Client();

  string Get_Mode(){ return mode; }
  
  float Request_HV_Control_Get(const string& parameter);
  void Request_HV_Control_Set(const string& parameter, const float& value);
  int Request_HV_Control_Status();
  
 protected:
  string mode;
  
  string foil_name;
  int channel;
  string tester;

  bool verbosity;

  string path;
  string path_fifo_client;
  string path_fifo_server;
  
  int fd_client;
  int fd_server;
  
  void Connect_To_Server();
  void Disconnect_From_Server();
  void Initialization();
  string Receive_From_Server();
  void Transmit_To_Server(const string& msg);
};
