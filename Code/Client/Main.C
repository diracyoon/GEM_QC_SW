#include <cstdlib>

#include "Client.h"

int main(int argc, char* argv[])
{
  if(argc!=5)
    {
      cerr << "Wrong number of argc." << endl;
      cout << "argv[1]: runnumber, argv[2]: foil name, argv[3]: channel number, argv[4]: tester" << endl;
      exit(1);
    }

  int runnumber = atoi(argv[1]);
  string foil_name = argv[2];
  int channel = atoi(argv[3]);
  string tester = argv[4];
  
  char* path = getenv("QC_SW_PATH");
  if(path==NULL)
    {
      cerr << "Enviromental variables aren't set. Source Env.sh first." << endl;
      return -1;
    }
  
  try
    {
      Client client(runnumber, foil_name, channel, tester, path);
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
