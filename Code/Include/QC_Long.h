#ifndef __QC_Long_h__
#define __QC_Long_h__

#include "QC_Base.h"

class QC_Long : public QC_Base
{
 public:
  QC_Long(const string& a_foil_name, const int& a_trial_number, const int& a_channel, const float& a_rh, const float& a_temp, const string& a_tester, const string& a_path, const bool& a_verbosity=false);
  ~QC_Long();

  void Run();
  
 protected:
  void Body();
};

#endif /* __QC_Long_h__ */
