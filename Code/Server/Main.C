#include <iostream>
//#include <cstdlib>

#include "Server.h"

using namespace std;

int main(int argc, char* argv[])
{
  // //init
  // char arg[64] = "ttyUSB2_9600_8_0_0_00";
  // int handle;
  // CAENHVRESULT hv_result = CAENHV_InitSystem((CAENHV_SYSTEM_TYPE_t)6, 5, arg, "", "", &handle);

  // if(hv_result!=CAENHV_OK)
  //   {
  //     cerr << CAENHV_GetError(handle) << endl;
  //     exit(1);
  //   }

  // //crate map
  // unsigned short n_slot;
  // unsigned short* n_ch;
  // char* model_list;
  // char* description_list;
  // unsigned short* serial_number_list;
  // unsigned char* firmware_release_min_list;
  // unsigned char* firmware_release_max_list;
  // CAENHV_GetCrateMap(handle, &n_slot, &n_ch, &model_list, &description_list, &serial_number_list, &firmware_release_min_list, &firmware_release_max_list);

  // cout << "Model = " << model_list << ", #Slot = " << n_slot << ", #Ch. = " << sizeof(n_ch)/sizeof(unsigned short) << endl;

  //make FIFO
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
