#include "Client.h"

//////////

Client::Client(const int& a_runnumber, const string& a_foil_name, const int& a_channel, const string& a_tester, const string& a_path, const bool& a_verbosity) : runnumber(a_runnumber), foil_name(a_foil_name), channel(a_channel), tester(a_tester), path(a_path), verbosity(a_verbosity)
{  
  Initialization();
}//Client::Client()

//////////

Client::~Client()
{
  close(fd_client);
  close(fd_server);

  unlink(path_fifo_client.c_str());

  result_out.close();
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
 
  path_fifo_client = path + "/Macro/FIFO"  + to_string(channel);
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
  path_fifo_server = path + "/Macro/FIFO";
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

void Client::Read_Config_Data()
{
  cout << "Read Config. data." << endl;
  
  ifstream fin_config;
  fin_config.open("/home/isyoon/GEM_QC2_Long_SW/Config/Config.dat");
  if(!fin_config.is_open()) throw "class Client: Can not find the config file.";

  string buf;
  while(!fin_config.eof())
    {
      getline(fin_config, buf);
      if(buf.empty()) break;
      
      istringstream iss(buf);

      Config_Data config_data;
      
      iss >> config_data.time;
      iss >> config_data.vset;

      vec_config_data.push_back(config_data);

      result_out << config_data.time << " " << config_data.vset << endl;
    }
  result_out << "##############################" << endl;
  
  return;
}//void Client::Read_Config_Data()

//////////

void Client::Recover_Trip(const float& vset)
{
  static int n_trip;
  n_trip++;

  cout << "class Client: Trip!! # of trip = " << n_trip << " Start recovery process." << endl;

  Request_HV_Control_Set("Pw", 0);
  Request_HV_Control_Set("Pw", 1);
  Request_HV_Control_Set("VSet", vset);
  

  float vmon_old = -1;
  while(1)
    {
      float vmon = Request_HV_Control_Get("VMon");

      cout << "Recovering trip. VSet: " << vset << ", VMon = " << vmon << endl;

      //voltage stabilization condition
      if(fabs(vmon-vset)/vset<0.05 && (vmon-vmon_old)<0.2)
	{
	  cout << "Recovery process done." << endl;
	  break;
	}

      vmon_old = vmon;
      
      this_thread::sleep_for(chrono::seconds(1));
    }
  
  
  return;
}//void Client::Recover_Trip()

//////////

void Client::Result_Log_Maker()
{
  path_result = path + "/Output/" + to_string(runnumber) + "_" + foil_name + ".result";
  result_out.open(path_result);

  result_out << "Runnumber: " << runnumber << endl;
  result_out << "Foil name: " << foil_name << endl;
  result_out << "Channel: " << runnumber << endl;
  result_out << "Tester: " << tester << endl;

  time_t time = system_clock::to_time_t(system_clock::now());
  result_out << "Start time: " << ctime(&time);
  result_out << "##############################" << endl;
  
  return;
}//void Client::Result_Log_Maker()

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

float Client::Scan_Config(const float& time_run)
{
  for(int i=0; i<vec_config_data.size(); i++)
    {
      if(vec_config_data[i].time<time_run && time_run<vec_config_data[i+1].time) return vec_config_data[i].vset;
    }

  return -1;
}//float Client::Scan_Config(const duration<float> time_run)

//////////

void Client::Transmit_To_Server(const string& msg)
{
  if(write(fd_server, msg.c_str(), MSG_SIZE)<0) throw "class Client: Can not write on FIFO for server.";

  if(verbosity) cout << "Client Ch" << channel << " say: " << msg << endl;

  return;
}//void Client::Transmit_To_Server(const string& msg)

//////////

void Client::QC_Long()
{
  QC_Long_Initialization();

  QC_Long_Body();
  
  //QC Long finalization - turn off
  Request_HV_Control_Set("Pw", 0);

  result_out << "##############################" << endl;
  time_t time = system_clock::to_time_t(system_clock::now());
  result_out << "End time: " << ctime(&time);

  cout << "QC Long done." << endl;
  
  return;
}//void Client::QC_Long()

//////////

void Client::QC_Long_Body()
{
  Result_Log_Maker();

  Read_Config_Data();
    
  cout << "Start QC2 Long test!!" << endl;

  float vset_old = -999;
  
  system_clock::time_point time_start = system_clock::now();

  while(1)
    {
      //run time
      system_clock::time_point time_current = system_clock::now();

      duration<float> time_run = time_current - time_start;
      
      float vset = Scan_Config(time_run.count());
      
      //end of QC
      if(vset<0) break;
      else if(vset!=vset_old)
	{
	  Request_HV_Control_Set("VSet", vset);
	  vset_old = vset;
	}
      
      if(Request_HV_Control_Get("Pw")==false) Recover_Trip(vset);
            
      float vmon = Request_HV_Control_Get("VMon");
      float imon = Request_HV_Control_Get("IMonH");

      result_out << time_run.count() << " " << vset << " " << vmon << " " << imon << endl; 

      if(int(time_run.count())%10==0) cout << "Run duration = " << time_run.count() << "s, V_set = " << vset << "V, V_Mon = " << vmon << "V, I_Mon = " << imon << "uA" << endl; 
      
      this_thread::sleep_for(chrono::seconds(1));
    }  
  
  return;
}//void Client::QC_Long_Body()

//////////

void Client::QC_Long_Initialization()
{
  cout << "Initialize QC_Long." << endl;

  Request_HV_Control_Set("Pw", 1);

  Request_HV_Control_Set("MaxV", 615);
  Request_HV_Control_Set("RUp", 1);
  Request_HV_Control_Set("RDwn", 50);

  Request_HV_Control_Set("ISet", 1);
  Request_HV_Control_Set("Trip", 0.1);
  Request_HV_Control_Set("PDwn", 0);
    
  return;
}//void Client::QC_Long_Initialization()

/////////
