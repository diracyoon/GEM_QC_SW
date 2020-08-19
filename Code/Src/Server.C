#include "Server.h"

//////////

Server::Server(const string& a_path_fifo, const string& a_port, const string& a_mode, const bool& a_verbosity) : hv_controller(a_port, a_mode), path_fifo_server(a_path_fifo), mode(a_mode), verbosity(a_verbosity)
{
  Initialization();
}//Server::Server()

//////////

Server::~Server()
{
  unlink(path_fifo_server.c_str());

  delete[] fd_client;
  delete[] path_fifo_client;
}//Server::~Server()

//////////

void Server::Run()
{
  cout << "Server::Server is running." << endl;

  while(1)
    {
      string msg = Receive_From_Client();

      //new connection
      if(msg.find("##HEAD##")!=string::npos) Register_Client(msg);

      //end connection
      if(msg.find("##END##")!=string::npos) Erase_Client(msg);
     
      //request HV get 
      if(msg.find("##GET##")!=string::npos) HV_Control_Get(msg);
      
      //request HV set
      if(msg.find("##SET##")!=string::npos) HV_Control_Set(msg);
      
      //request HV get status
      if(msg.find("##STATUS##")!=string::npos) HV_Control_Status(msg);
      
      //this_thread::sleep_for(chrono::milliseconds(10));
    }

  return;
}//void Server::Run()

//////////

void Server::Erase_Client(const string& msg)
{
  int channel = stoi(msg.substr(10, 1), nullptr);

  cout << "Server::Erase client #Ch." << channel << endl;

  if(close(fd_client[channel])<0) throw "class Server: Cat not close FIFO for client.";
  fd_client[channel] = -1;
  path_fifo_client[channel].clear();
  
  return;
}//void Server::Erase_Client(const string& msg)

//////////

void Server::HV_Control_Get(const string& msg)
{
  istringstream iss(msg);

  string buf;
  iss >> buf;
  iss >> buf;

  int channel;
  channel = stoi(buf.substr(2, 1), NULL);

  string parameter;
  iss >> parameter;

  float value = 0;

  //return null value when DEBUG mode is set
  if(mode.compare("DEBUG")==0)
    {
      Transmit_To_Client(channel, "##OK## " + to_string(value));

      return;
    }
  
  CAENHVRESULT result = hv_controller.Get(channel, parameter, value);
  
  if(result==CAENHV_OK) Transmit_To_Client(channel, "##OK## " + to_string(value));
  else Transmit_To_Client(channel, "##BAD##");

  return;
}//void Server::HV_Control_Get(const string& msg)

//////////

void Server::HV_Control_Set(const string& msg)
{
  istringstream iss(msg);

  string buf;
  iss >> buf;
  iss >> buf;

  int channel;
  channel = stoi(buf.substr(2, 1), NULL);
  
  string parameter; 
  iss >> parameter;

  float value;
  iss >> value;

  //return null value when DEBUG mode is set
  if(mode.compare("DEBUG")==0)
    {
      Transmit_To_Client(channel, "##OK##");
      
      return;
    }
  
  CAENHVRESULT result = hv_controller.Set(channel, parameter, value);

  if(result==CAENHV_OK) Transmit_To_Client(channel, "##OK##");
  else Transmit_To_Client(channel, "##BAD##");
    
  return;
}//void Server::HV_Control_Set(const string& msg)

//////////

void Server::HV_Control_Status(const string& msg)
{
  istringstream iss(msg);

  string buf;
  iss >> buf;
  iss >> buf;

  int channel;
  channel = stoi(buf.substr(2, 1), NULL);

  int value = 0;

  //return null value when DEBUG mode is set
  if(mode.compare("DEBUG")==0)
    {
      Transmit_To_Client(channel, "##OK## " + to_string(value));
      
      return;
    }
  
  CAENHVRESULT result = hv_controller.Status(channel, value);
  
  if(result==CAENHV_OK) Transmit_To_Client(channel, "##OK## " + to_string(value));
  else Transmit_To_Client(channel, "##BAD##");
  
  return;
}//void Server::HV_Control_Status(const string& msg)

//////////

void Server::Initialization()
{
  cout << "Server::Initialize server." << endl;
  cout << "Server::Mode = " << mode << endl;
  cout << "Server::Start Initialization." << endl;

  //make FIFO for server
  if(mkfifo(path_fifo_server.c_str(), 0666)==-1) throw "class Server: Can not make FIFO for server. Remove previous FIFO first.";

  //open FIFO
  fd_server = open(path_fifo_server.c_str(), O_RDWR);
  if(fd_server<0) throw "class Server: Can not open FIFO for server.";

  //obtain max channel
  max_channel = hv_controller.Get_Total_Channel_Number();
  cout << "Max #Ch.: " << max_channel << endl;
  
  //memory allocation for client handling
  fd_client = new int[max_channel];
  path_fifo_client = new string[max_channel];

  cout << "Server::Initialization done." << endl;
  
  return;  
}//void Server::Initionalization()

//////////

string Server::Receive_From_Client()
{
  char receive[MSG_SIZE];
  if(read(fd_server, receive, MSG_SIZE)<0) throw "class Server: Can not read FIFO for server.";

  string buf = receive;

  if(verbosity) cout << "Client say: " << buf << endl;
  
  return buf;
}

//////////

void Server::Register_Client(const string& msg)
{
  int channel = stoi(msg.substr(11, 1), nullptr);

  //out of channel
  if(max_channel<=channel)
    {
      cerr << "Server say: Out of HV channel range. Ignore" << endl;
      return;
    }
      
  path_fifo_client[channel] = msg.substr(13, msg.size()-13);

  cout << "Register client #Ch." << channel << endl;
  
  //fd_client[channel] = open(path_fifo_client[channel].c_str(), O_WRONLY);
  fd_client[channel] = open(path_fifo_client[channel].c_str(), O_RDWR);
  
  if(fd_client[channel]<0)
    {
      string error = "class Server: Fail to open FIFO for client #Ch." + to_string(channel) + ".";
      throw error.c_str();
    }

  string transmit = "##OK## Successfully registor client #Ch." + to_string(channel) +
    " Mode=" + mode;

  Transmit_To_Client(channel, transmit);
  
  return;
}//void Server::Register_Client()

//////////

void Server::Transmit_To_Client(const int& channel, const string& msg)
{
  if(write(fd_client[channel], msg.c_str(), MSG_SIZE)<0)
    {
      string error = "class Server: Fail to write FIFO for client #Ch." + to_string(channel) + ".";
      throw error;
  }

  if(verbosity) cout << "Server say: " << msg << endl;
  
  return;
}//void Server::Transmit_To_Client(const string& msg)

//////////
