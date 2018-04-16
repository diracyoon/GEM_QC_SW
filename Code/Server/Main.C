#include "Server.h"

using namespace std;

int main(int argc, char* argv[])
{
  try
    {
      Server server("/home/isyoon/Test_HV_Control/Server/FIFO");
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
