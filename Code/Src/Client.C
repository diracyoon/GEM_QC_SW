#include "Client.h"

//////////

Client::Client(const int& a_channel, const string& a_path, const bool& a_verbosity) : channel(a_channel), path(a_path), verbosity(a_verbosity)
{  
  Initialization();
  Connect_To_Server();
}//Client::Client()

//////////

Client::~Client()
{
  Disconnect_From_Server();
    
  close(fd_client);
  close(fd_server);

  unlink(path_fifo_client.c_str());
}//Client::~Client()

//////////

float Client::Request_HV_Control_Get(const string& parameter)
{
  for(int i=0; i<2; i++)
    {
      string transmit = "##GET## CH" + to_string(channel) + " " + parameter;
      Transmit_To_Server(transmit);

      string result = Receive_From_Server();

      if(result.find("##OK##")!=string::npos)
	{
	  istringstream iss(result);

	  string buf;
	  iss >> buf;

	  float value;
	  iss >> value;

	  return value;
	}

      //if the first trial failed, wait 1 second and try one more time.
      this_thread::sleep_for(chrono::seconds(1));
    }

  throw "class Client: HV control request failed.";
}//bool Client::Request_HV_Control_Get(const string& parameter, int& value)

//////////

void Client::Request_HV_Control_Set(const string& parameter, const float& value)
{
  for(int i=0; i<2; i++)
    {
      string transmit = "##SET## CH" + to_string(channel) + " " + parameter + " " + to_string(value);
      Transmit_To_Server(transmit);

      string result = Receive_From_Server();

      if(result.find("##OK##")!=string::npos) return;

      //if the first trial failed, wait 1 second and try one more time.
      this_thread::sleep_for(chrono::seconds(1));
    }

  throw "class Client: HV control request failed.";
}//void Client::Request_HV_Control_Set(const string& target, const int& value)

//////////

int Client::Request_HV_Control_Status()
{
  for(int i=0; i<10; i++)
    {
      string transmit = "##STATUS## CH" + to_string(channel);
      Transmit_To_Server(transmit);

      string result = Receive_From_Server();

      if(result.find("##OK##")!=string::npos)
	{
	  istringstream iss(result);

	  string buf;
	  iss >> buf;

	  int value;
	  iss >> value;

	  return value;
	}

      //if the first trial failed, wait 1 second and try one more time.
      this_thread::sleep_for(chrono::seconds(1));
    }

  throw "class Client: HV control request failed.";
  
}//float Client::Request_HV_Control_Status()

//////////

void Client::Connect_To_Server()
{
  cout << "Client: Connect to server." << endl;
  
  string msg = "##HEAD## CH" + to_string(channel) + " " + path_fifo_client;
  Transmit_To_Server(msg);
  
  msg = Receive_From_Server();

  //mode
  mode = msg.substr(msg.find_last_of("=")+1);

  return;
}//void Client::Connect_To_Server()

//////////

void Client::Disconnect_From_Server()
{
  cout << "Client: Disconnect from server." << endl;

  string msg = "##END## CH" + to_string(channel);
  Transmit_To_Server(msg);
  
  return;
}//void Client::Disconnect_From_Server()

//////////

void Client::Initialization()
{
  cout << "Client: Initialize client for Ch." << channel << endl;

  //make FIFO for client
  path_fifo_client = path + "/FIFOs/FIFO"  + to_string(channel);
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
  path_fifo_server = path + "/FIFOs/FIFO";
  fd_server = open(path_fifo_server.c_str(), O_WRONLY);//, O_NONBLOCK);   
  if(fd_server<0) throw "class Client: Can not open FIFO for server.";
  
  return;
}//void Client::Initialization()

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
  
  if(verbosity) cout << "Server say:     " << msg << endl;
      
  return msg;
}//string Client::Receive_From_Server()

//////////

void Client::Transmit_To_Server(const string& msg)
{
  if(write(fd_server, msg.c_str(), MSG_SIZE)<0) throw "class Client: Can not write on FIFO for server.";

  if(verbosity) cout << "Client Ch" << channel << " say: " << msg << endl;

  return;
}//void Client::Transmit_To_Server(const string& msg)
