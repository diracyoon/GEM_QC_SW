#include "Painter.h"

//////////

Painter::Painter(const string& a_file_path) : file_path(a_file_path)
{
  fin.open(file_path);
  
  istringstream iss;
  string buf;

  //parsing
  //runnumber
  getline(fin, buf);
  iss.str(buf);
  iss >> buf;
  iss >> runnumber;

  //foil name
  getline(fin, buf);
  iss.clear();
  iss.str(buf);
  iss >> buf;
  iss >> buf;
  iss >> foil_name;
    
  //channel
  getline(fin, buf);
  iss.clear();
  iss.str(buf);
  iss >> buf;
  iss >> channel;

  //relative humidity
  getline(fin, buf);
  iss.clear();
  iss.str(buf);
  iss >> buf;
  iss >> buf;
  iss >> rh;
  
  //temperature
  getline(fin, buf);
  iss.clear();
  iss.str(buf);
  iss >> buf;
  iss >> temperature;

  int chk_separater = 0;
  while(1)
    {
      getline(fin, buf);

      if(buf.find("##")!=string::npos) chk_separater++;

      if(chk_separater==2) break;
    }
  
  gr_vset = new TGraphErrors();
  gr_vset->SetName("V_Set");
  gr_vset->SetTitle("Time vs. V_{Set}");
  gr_vset->GetXaxis()->SetTitle("Time [s]");
  gr_vset->GetYaxis()->SetTitle("V_{Set} [V]");
  
  gr_vmon = new TGraphErrors();
  gr_imon = new TGraphErrors();
}//Painter::Painter()

//////////

Painter::~Painter()
{
  string can_name = to_string(runnumber) + "_" + foil_name;
  TCanvas* can_self = new TCanvas(can_name.c_str(), can_name.c_str(), 800, 500);
  can_self->Draw();

  Draw(can_self);

  string print_name = file_path.substr(0, file_path.find_last_of("/")) + "/" + can_name + ".png";
  
  can_self->Print(print_name.c_str(), "png");
  
  delete gr_vset;
  delete gr_vmon;
  delete gr_imon;
  delete can_self;
}//Painter::~Painter()

//////////

void Painter::Draw(TVirtualPad* pad)
{
  pad->cd();

  gr_vset->Draw("AP*");
}//void Painter::Draw(TVirtualPad* pad) 

//////////

void Painter::Update()
{
  fin.clear();

  string buf;
  while(fin.eof()==false)
    {
      getline(fin, buf);

      if(buf.find("##")!=string::npos) break;
      if(buf.compare("")==0) break;
      
      //parsing
      istringstream iss;
      iss.str(buf);

      float time;
      iss >> time;

      float vset;
      iss >> vset;

      float vmon;
      iss >> vmon;

      float imon;
      iss >> imon;

      //add point to graphs
      int n_point = gr_vset->GetN();

      gr_vset->SetPoint(n_point, time, vset);

      gr_vmon->SetPoint(n_point, time, vmon);
      
      gr_imon->SetPoint(n_point, time, imon);
    }
  
  return;
}//void Painter::Update()
  
///////////
