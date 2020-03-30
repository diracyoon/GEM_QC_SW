#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sstream>

#include <fcntl.h>
#include <chrono>
#include <thread>
#include <string>

#include <CAENHVWrapper.h>

#include "HV_Controller.h"

#define MSG_SIZE 64

using namespace std;

class HV_Contoller;

class Server
{
 public:
  Server(const string& a_path_fifo, const string& a_port="test_port", const string& a_mode="NORMAL", const bool& a_verbosity=false);
  ~Server();

  void Run();
  
 protected:
  HV_Controller hv_controller;
  
  string path_fifo_server;

  string mode;
  bool verbosity;
  
  int fd_server;

  int max_channel;

  int* fd_client;
  string* path_fifo_client;
  
  void Erase_Client(const string& msg);
  void HV_Control_Get(const string& msg);
  void HV_Control_Set(const string& msg);
  void HV_Control_Status(const string& msg);
  void Initialization();
  string Receive_From_Client();
  void Register_Client(const string& msg);
  void Transmit_To_Client(const int& channel, const string& msg);  
};
