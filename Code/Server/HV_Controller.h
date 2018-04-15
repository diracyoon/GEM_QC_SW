#include <string>

#include <CAENHVWrapper.h>

using namespace std;

class HV_Controller
{
 public:
  HV_Controller();
  ~HV_Controller();

  CAENHVRESULT Set(const int& channel, const string& parameter, const float& value);
 protected:
};
