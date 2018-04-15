#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <unistd.h>
#include <fcntl.h>

#include "Client.h"

using namespace std;

int main(int argc, char* argv[])
{
  if(argc!=2)
    {
      cerr << "Wrong number of argc." << endl;
      exit(1);
    }
  
  int channel = atoi(argv[1]);

  try
    {
      Client client("/home/isyoon/Test_HV_Control/Server/FIFO", channel);
      client.Run();	
    }
  catch(const string& error)
    {
      cerr << error << endl;
    }
  catch(const char* error)
    {
      cerr << error << endl;
    }
  

  // while(1)
  //   {
  //     if(read(file_des_client, receive, 64)<0)
  //     {
  // 	cerr << "Can not read client FIFO." << endl;
  // 	exit(1);
  //     }

  //     cout << "Server say: " <<  receive << endl;
  //     string buf = receive;
  //     if(buf.substr(0, 12).compare("Successfully")==0)  break;
      
  //     sleep(0.1);
  //   }
  
  // while(1)
  //   {
  //     cout << "input transmit" << endl;
  //     cin >> transmit;
      
  //     if(write(file_des_server, transmit, 64)==-1)
  // 	{
  // 	  cerr << "fail to write." << endl;
  // 	  exit(1);
  // 	}
  //   }
  // //sleep(1);
  
  return 0;
}
