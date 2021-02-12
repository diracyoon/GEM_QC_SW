#include <cstdlib>
#include <iostream>

#include "Server.h"

using namespace std;

int main(int argc, char* argv[])
{
  if(argc!=2&&argc!=3&&argc!=4)
    {
      cout << "Wrong number of argc." << endl;
      cout << "argv[1]: port i.e. ttyUSB0." << endl;
      cout << "argv[2]: mode i.e. NORMAL or DEBUG. Default is NORMAL." << endl;
      cout << "argv[3]: verbosity i.e. true or false. Default is false." << endl;
      exit(1);
    }
  
  char* path = getenv("QC_SW_PATH");
  if(path==NULL)
    {
      cout << "Enviromental variables aren't set. Source Env.sh first." << endl;

      return -1;
    }

  string fifo_path = path;
  fifo_path += "/FIFOs/FIFO";

  string port = argv[1];

  string mode;

  if(argc==2) mode = "NORMAL";
  else if(argc==3) mode = argv[2];

  try
    {
      Server server(fifo_path, port, mode);
      server.Run();
    }
  catch(const string& error)
    {
      cout << error << endl;
    }
  catch(const char* error)
    {
      cout << error << endl;
    }
    
  return 0;
}
