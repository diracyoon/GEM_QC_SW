#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

class Submitter
{
 public:
  Submitter(const string& foil_name, const string& a_process, const int& trial_number);
  ~Submitter();

 protected:
  string foil_name;
  string process;
  int trial_number;
};
