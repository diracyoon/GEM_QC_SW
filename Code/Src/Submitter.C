#include "Submitter.h"

//////////

Submitter::Submitter(const string& a_foil_name, const string& a_process, const int& a_trial_number) : foil_name(a_foil_name), process(a_process), trial_number(a_trial_number) 
{
  string hostname = "147.46.50.23";
  string port = "31417";

  string buf = foil_name;
  int underbar[2] = {0};
  for(int i=0; i<2; i++)
    {
      underbar[i] = buf.find_first_of("_");

      buf = buf.substr(underbar[i]+1, buf.size());
    }
  underbar[1] += underbar[0] + 1;
  
  string foil_type = foil_name.substr(0, underbar[0]);
  
  process = process.substr(process.find_last_of("/")+1);

  working_dir = getenv("QC_SW_PATH");
  working_dir += "/Output/" + process + "/" + foil_name + "/";
  
  string path = working_dir + foil_name + "_"  + string(2 - to_string(trial_number).length(), '0') + to_string(trial_number); 
  
  string command = "elog";

  //hostname 
  command += " -h " + hostname;

  //port
  command += " -p " + port;

  //ssl
  command += " -s";

  //encoding
  command += " -n 1";

  //ID and passward 
  command += " -u GEMQCSW GEMQCSW";
  
  //type
  command += " -l " + foil_type;

  //author
  command += " -a Author=GEMQCSW";

  //process
  if(process.compare("Preparation_QC_Long")==0) command += " -a type=Preparation_QC";
  else if(process.compare("QC_Long")==0) command += " -a type=QC_Long";

  //batch id
  string batch = foil_name.substr(underbar[0]+1, underbar[1]+1);
  command += " -a Batch_ID=" + batch;

  //foild id
  string foil_id = foil_name.substr(underbar[1]+1, foil_name.size());
  command += " -a Foil_ID=" + foil_id;

  //subject
  string subject = foil_name + "_" + to_string(trial_number);
  command += " -a Subject=" + subject;

  //message file
  MSG_Maker();

  command += " -m " + working_dir + "MSG";

  //result png file
  command += " -f " + path + ".png";
  
  //result tex file
  command += " -f " + path + ".result";

  cout << command << endl;

  system(command.c_str());
}//Submitter::Submitter(const Watch_Result& a_watch_result)

//////////

Submitter::~Submitter()
{
}//Submitter::~Submitter()

//////////

void Submitter::MSG_Maker()
{
  msg.open(working_dir+"MSG");
  msg << foil_name << endl;
  msg << trial_number << endl;

  msg.close();
  
  return;
}//void Submitter::MSG_Maker()

//////////
