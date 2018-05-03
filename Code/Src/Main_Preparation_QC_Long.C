#include <cstdlib>
#include <iostream>

#include "Preparation_QC_Long.h"

using namespace std;

int main(int argc, char* argv[])
{
  if(argc!=7)
    {
      cerr << "Wrong number of argc." << endl;
      cout << "argv[1]: runnumber, argv[2]: foil name, argv[3]: channel number, argv[4]: relative humidity, argv[5]: temperature, argv[6]: tester" << endl;
      exit(1);
    }

  int runnumber = atoi(argv[1]);
  string foil_name = argv[2];
  int channel = atoi(argv[3]);
  float rh = atof(argv[4]);
  float temp = atof(argv[5]);
  string tester = argv[6];
  
  char* path = getenv("QC_SW_PATH");
  if(path==NULL)
    {
      cerr << "Enviromental variables aren't set. Source Env.sh first." << endl;
      return -1;
    }
  
  try
    {
      Preparation_QC_Long preparation_qc_long(runnumber, foil_name, channel, rh, temp, tester, path);
      preparation_qc_long.Run();
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
