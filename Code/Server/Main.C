#include <cstdlib>
#include <iostream>

#include "Server.h"

using namespace std;

int main(int argc, char* argv[])
{
  if(argc!=2)
    {
      cerr << "Wrong number of argc." << endl;
      cout << "argv[1]: port i.e. ttyUSB0." << endl;
      exit(1);
    }
  
  char* path = getenv("QC_SW_PATH");
  if(path==NULL)
    {
      cerr << "Enviromental variables aren't set. Source Env.sh first." << endl;

      return -1;
    }

  string fifo_path = path;
  fifo_path += "/Macro/FIFO";

  string port = argv[1];
  
  try
    {
      Server server(fifo_path, port);
      server.Run();
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
