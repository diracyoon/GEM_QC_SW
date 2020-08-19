#include "HV_Controller.h"

//////////

HV_Controller::HV_Controller(const string& a_port, const string& a_mode) : port(a_port), mode(a_mode)
{
  if(mode.compare("NORMAL")==0)
    {
      Make_Connection();
      Create_HV_Map();
    }
}//HV_Controller::HV_Controller()

//////////

HV_Controller::~HV_Controller()
{
}//HV_Controller::~HV_Controller()

//////////

CAENHVRESULT HV_Controller::Get(const unsigned short& channel, const string& parameter, float& value)
{
  if(mode.compare("DEBUG")==0)
    {
      //null value
      value = 0;

      CAENHVRESULT result;
      return result;
    }//debug mode
  
  else
    {  
      unsigned short ch_list[1] = {channel};
      
      CAENHVRESULT result;
      if(parameter.compare("Pw")==0)
	{
	  bool value_list[1];
	  result = CAENHV_GetChParam(handle, 0, parameter.c_str(), 1, ch_list, value_list);
	  value = value_list[0];
	}
      else
	{
	  float value_list[1];
	  result = CAENHV_GetChParam(handle, 0, parameter.c_str(), 1, ch_list, value_list);
	  value = value_list[0];
	}
      
      return result;
    }//normal mode
  
}//CAENHVRESULT HV_Controller::Get(const int& channel, const string& parameter, const float& value)

//////////

CAENHVRESULT HV_Controller::Set(const unsigned short& channel, const string& parameter, const float& value)
{
  if(mode.compare("DEBUG")==0)
    {
      CAENHVRESULT result;
      return result;
    }//debug mode

  else
    {
      unsigned short ch_list[1] = {channel};

      CAENHVRESULT result;
      if(parameter.compare("ImonRange")==0)
	{
          int value_list[1] = {(int)value};
	  result = CAENHV_SetChParam(handle, 0, parameter.c_str(), 1, ch_list, value_list);
	}
      else
	{
	  float value_list[1] = {value};
	  result = CAENHV_SetChParam(handle, 0, parameter.c_str(), 1, ch_list, value_list);
	}
      
      return result;
    }//normal mode
}//CAENHVRESULT HV_Controller::Set(const int& channel, const string& parameter, const float& value)

CAENHVRESULT HV_Controller::Status(const unsigned short& channel, int& value)
{
  if(mode.compare("DEBUG")==0)
    {
      CAENHVRESULT result;
      return result;
    }//debug mode

  else
    {
      unsigned short ch_list[1] = {channel};

      unsigned int value_list[1];
      CAENHVRESULT result = CAENHV_GetChParam(handle, 0, "ChStatus", 1, ch_list, value_list);
      
      value = value_list[0];
      
      return result;
    }
  
}//CAENHVRESULT HV_Controller::Status(const unsigned short& channel, int& value)

//////////

int HV_Controller::Get_Total_Channel_Number()
{
  if(mode.compare("DEBUG")==0) return 4;
  else return total_ch;
}//int HV_Controller::Get_Total_Channel_Number()

//////////

void HV_Controller::Create_HV_Map()
{
  total_ch = 0;
  
  CAENHV_GetCrateMap(handle, &n_slot, &n_ch, &model_list, &description_list, &serial_number_list, &firmware_release_min_list, &firmware_release_max_list);

  cout << "Model = " << model_list << ", #Slot = " << n_slot << endl;
  cout << "#Ch:"  << endl;
  for(int i=0; i<n_slot; i++)
    {
      cout << "Slot " << i << ": " << n_ch[i] << endl;
      total_ch += n_ch[i];
    }
  
  return;
}//void HV_Controller::Cerate_HV_Map()

//////////

void HV_Controller::Make_Connection()
{
  char arg[64];
  sprintf(arg, "%s_9600_8_0_0_00", port.c_str());;
 
  CAENHVRESULT hv_result = CAENHV_InitSystem((CAENHV_SYSTEM_TYPE_t)6, 5, arg, "", "", &handle);

  if(hv_result!=CAENHV_OK) throw "class HV_Connection: Can not make a connection to HV module.";
    
  return;
}//void HV_Controller::Make_Connection()

//////////
