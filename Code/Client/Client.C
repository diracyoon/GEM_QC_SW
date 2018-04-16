#include "Client.h"

//////////

Client::Client(const string& a_path_fifo_server, const int& a_channel) : path_fifo_server(a_path_fifo_server), channel(a_channel)
{
  Initialization();
}//Client::Client()

//////////

Client::~Client()
{
  close(fd_client);
  close(fd_server);

  unlink(path_fifo_client.c_str());
}//Client::~Client()

//////////

void Client::Run()
{
  
  Connect_To_Server();

  //start test
  QC_Long();
    
  Disconnect_From_Server();
  
  return;
}

//////////

void Client::Connect_To_Server()
{
  cout << "Connect to server." << endl;
  
  string msg = "##HEAD## CH" + to_string(channel) + " " + path_fifo_client;
  Transmit_To_Server(msg);
  
  msg = Receive_From_Server();
  
  return;
}//void Client::Connect_To_Server()

//////////

void Client::Disconnect_From_Server()
{
  cout << "Disconnect from server." << endl;

  string msg = "##END## CH" + to_string(channel);
  Transmit_To_Server(msg);
  
  return;
}//void Client::Disconnect_From_Server()

//////////

void Client::Initialization()
{
  cout << "Initialize client for Ch." << channel << endl;

  //make FIFO for client
  path_fifo_client = "/home/isyoon/Test_HV_Control/Client/FIFO" + to_string(channel);
  
  if(mkfifo(path_fifo_client.c_str(), 0666)==-1)
    {
      string error = "class Client: Can not make FIFO for client Ch" + to_string(channel) + ". Remove previous FIFO first.";
      throw error;
    }
  
  //open FIFO for client
  fd_client = open(path_fifo_client.c_str(), O_RDWR);
  if(fd_client<0)
    {
      string error = "class Client: Can not open FIFO for client Ch" + to_string(channel) + ".";
      throw error;
    }
  
  //open fifo for server
  fd_server = open(path_fifo_server.c_str(), O_WRONLY);//, O_NONBLOCK);   
  if(fd_server<0) throw "class Client: Can not open FIFO for server.";
  
  return;
}//void Client::Initialization()

//////////

void Client::Initialization_QC()
{
  cout << "Initialize QC_Long." << endl;
  
  //trun on power
  Request_HV_Control_Set("Pw", 1);
  
  return;
}//void Client::Initialization_QC()

//////////

string Client::Receive_From_Server()
{
  char receive[MSG_SIZE];

  if(read(fd_client, receive, MSG_SIZE)<0)
    {
      string error = "class Client: Can not read FIFO for client Ch" + to_string(channel) + ".";
      throw error;
    }

  string msg = receive;
  
  cout << "Server say:     " << msg << endl;
      
  return msg;
}//string Client::Receive_From_Server()

//////////

void Client::Read_Config_Data()
{
  ifstream fin_config;
  fin_config.open("/home/isyoon/GEM_QC2_Long_SW/Config");
  if(!fin_config.is_open()) throw "class Client: Can not find the config file.";

  string buf;
  while(!fin_config.eof())
    {
      getline(fin_config, buf);
      if(buf.empty()) break;
      
      istringstream iss(buf);

      int time;
      int vset;
      
      buf >> time;
      buf >> vset;

      Config_Data config_data;
      vec_config_data.push_back(config_data);
    }
  
  return;
}//void Client::Read_Config_Data()

//////////

bool Client::Request_HV_Control_Get(const string& parameter, float& value)
{
  string transmit = "##GET## CH" + to_string(channel) + " " + parameter;
  Transmit_To_Server(transmit);

  string result = Receive_From_Server();
  
  if(result.find("##OK##")!=string::npos)
    {
      istringstream iss(result);

      string buf;
      iss >> buf;

      iss >> value;
      
      return true;
    }
  else return false;
}//bool Client::Request_HV_Control_Get(const string& parameter, int& value)

//////////

bool Client::Request_HV_Control_Set(const string& parameter, const float& value)
{
  string transmit = "##SET## CH" + to_string(channel) + " " + parameter + " " + to_string(value);
  Transmit_To_Server(transmit);

  string result = Receive_From_Server();

  if(result.find("##OK##")!=string::npos) return true;
  else return false;
}//void Client::Request_HV_Control_Set(const string& target, const int& value)

//////////

void Client::Transmit_To_Server(const string& msg)
{
  if(write(fd_server, msg.c_str(), MSG_SIZE)<0) throw "class Client: Can not write on FIFO for server.";

  cout << "Client Ch" << channel << " say: " << msg << endl;

  return;
}//void Client::Transmit_To_Server(const string& msg)

//////////

void Client::QC_Long()
{
  Initialization_QC();

  QC_Long_Body();
  
  //finalize QC - turn off
  Request_HV_Control_Set("Pw", 0);

  return;
}//void Client::QC_Long()

//////////

void Client::QC_Long_Body()
{
  Read_Config_Data();
  
  system_clock::time_point time_start = system_clock::now();
  
  while(1)
    {
      system_clock::time_point time_current = system_clock::now();;

      duration<float> time_run = time_current - time_start;
      cout << time_run.count() << endl;
      
      float value;
      Request_HV_Control_Get("IMonL", value);

      this_thread::sleep_for(chrono::seconds(1));
    }  
  
  return;
}//void Client::QC_Long_Body()

//////////
