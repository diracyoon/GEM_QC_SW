#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MSG_SIZE 64

using namespace std;

class Client
{
 public:
  Client(const string& a_path_fifo_server, const int& a_channel);
  ~Client();

  void Run();
  
 protected:
  int channel;

  string path_fifo_client;
  string path_fifo_server;
  
  int fd_client;
  int fd_server;

  void Connect_To_Server();
  void Disconnect_From_Server();
  void Initialization();
  void Initialization_QC();
  string Receive_From_Server();
  bool Request_HV_Control_Get();
  bool Request_HV_Control_Set(const string& parameter, const int& value);
  void Transmit_To_Server(const string& msg);
  void QC_Long();
};
