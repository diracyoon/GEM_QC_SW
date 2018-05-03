#include <string>
#include <iostream>

#include <CAENHVWrapper.h>

using namespace std;

class HV_Controller
{
 public:
  HV_Controller(const string& a_port);
  ~HV_Controller();

  CAENHVRESULT Get(const unsigned short& channel, const string& parameter, float& value);
  CAENHVRESULT Set(const unsigned short& channel, const string& parameter, const float& value);

 protected:
  string port;

  int handle;

  unsigned short n_slot;
  unsigned short* n_ch;
  char* model_list;
  char* description_list;
  unsigned short* serial_number_list;
  unsigned char* firmware_release_min_list;
  unsigned char* firmware_release_max_list;
    
  void Create_HV_Map();
  void Make_Connection();
};
