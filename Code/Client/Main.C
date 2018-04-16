#include "Client.h"

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
  
  return 0;
}
