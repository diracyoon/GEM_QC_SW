#ifndef __Preparation_QC_Long_h__
#define __Preparation_QC_Long_h__

#include <iomanip>

#include "QC_Base.h"

class Preparation_QC_Long : public QC_Base
{
 public:
  Preparation_QC_Long(const string& a_foil_name, const int& a_trial_number, const int& a_channel, const float& a_rh, const float& a_temp, const string& a_tester, const string& a_path, const bool& a_verbosity=false);
  ~Preparation_QC_Long();

  void Run();
  
 protected:
  void Body(); 
  virtual void Initialization_HV();
};

#endif /* __Preparation_QC_Long_h__ */
