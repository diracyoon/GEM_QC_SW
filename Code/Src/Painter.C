#include "Painter.h"

//////////

Painter::Painter(const string& a_file_path) : file_path(a_file_path)
{
  string buf = file_path.substr(0, file_path.find_last_of('/'));
  process = buf.substr(buf.find_last_of('/')+1, buf.size());
  
  fin.open(file_path);
  
  istringstream iss;
  
  //parsing

  //foil name
  getline(fin, buf);
  iss.clear();
  iss.str(buf);
  iss >> buf;
  iss >> buf;
  iss >> foil_name;

  //trial number
  getline(fin, buf);
  iss.clear();
  iss.str(buf);
  iss >> buf;
  iss >> buf;
  iss >> trial_number;
  
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
  iss >> buf;
  iss >> rh;
  
  //temperature
  getline(fin, buf);
  iss.clear();
  iss.str(buf);
  iss >> buf;
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
  gr_vmon = new TGraphErrors();
  gr_imon = new TGraphErrors();
  gr_imon_scale = new TGraphErrors();
  
  //graph style
  gr_vset->SetTitle(Form("Foil: %s, Process: %s", foil_name.c_str(), process.c_str()));
  gr_vset->GetXaxis()->SetTitle("Time (s)");
  gr_vset->GetXaxis()->SetTitleSize(0.04);
  gr_vset->GetXaxis()->SetLabelSize(0.04);
  gr_vset->GetYaxis()->SetTitle("Voltage (V)");
  gr_vset->GetYaxis()->SetTitleSize(0.04);
  gr_vset->GetYaxis()->SetLabelSize(0.04);
    
  gr_vset->SetMarkerStyle(20);
  gr_vset->SetMarkerColor(kRed);
  gr_vset->SetLineColor(kRed);
  gr_vset->SetLineWidth(2);
  gr_vset->SetMarkerStyle(34);
  gr_vset->SetMarkerColor(kRed);
  
  gr_vmon->SetMarkerStyle(20);
  gr_vmon->SetMarkerColor(kOrange);
  gr_vmon->SetLineColor(kOrange);
  gr_vmon->SetLineWidth(2);
  gr_vmon->SetMarkerStyle(29);
  gr_vmon->SetMarkerColor(kOrange);
  
  gr_imon_scale->SetMarkerStyle(20);
  gr_imon_scale->SetMarkerColor(kBlue);
  gr_imon_scale->SetLineColor(kBlue);
  gr_imon_scale->SetLineWidth(2);
  gr_imon_scale->SetMarkerStyle(20);
  gr_imon_scale->SetMarkerColor(kBlue);
  
  //2nd yaxis
  axis = new TGaxis(0, 0, 0, 0, 0, 0, 510, "+L");

  axis->SetTitle("Current (#muA)");
  axis->SetTitleSize(0.04);
  axis->SetTitleFont(42);
  axis->SetLabelSize(0.04);
  axis->SetLabelFont(42);
  
  //addtional text
  string text = "Trial number: " + to_string(trial_number);
  latex0 = new TLatex(.13, .85, text.c_str());
  latex0->SetNDC();
  latex0->SetTextSize(0.04);
  latex0->SetTextAlign(12);
  latex0->SetTextFont(42);

  text = "Relative humidity: " + to_string(rh).substr(0, to_string(rh).find_last_of('.')+2) + "%";
  latex1 = new TLatex(.13, .80, text.c_str());
  latex1->SetNDC();
  latex1->SetTextSize(0.04);
  latex1->SetTextAlign(12);
  latex1->SetTextFont(42);

  text = "Temperature: " + to_string(temperature).substr(0, to_string(temperature).find_last_of('.')+2) + "#circC";
  latex2 = new TLatex(.13, .75, text.c_str());
  latex2->SetNDC();
  latex2->SetTextSize(0.04);
  latex2->SetTextAlign(12);
  latex2->SetTextFont(42);
  
  //legend
  legend = new TLegend(.13, .50, .23, .70);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->AddEntry(gr_vset, "V_{set}", "lp");
  legend->AddEntry(gr_vmon, "V_{mon}", "lp");
  legend->AddEntry(gr_imon_scale, "I_{mon}", "lp");
}//Painter::Painter()

//////////

Painter::~Painter()
{
  this->Update();
  
  string can_name = foil_name;
  can_name += "_" + string(2 - to_string(trial_number).length(), '0') + to_string(trial_number);
  
  TCanvas* can_self = new TCanvas(can_name.c_str(), can_name.c_str(), 800, 500);
  can_self->Draw();

  Draw(can_self);

  string print_name = file_path.substr(0, file_path.find_last_of("/")) + "/" + can_name + ".png";
  
  can_self->Print(print_name.c_str(), "png");

  delete gr_vset;
  delete gr_vmon;
  delete gr_imon;
  delete gr_imon_scale;
  delete axis;
  delete latex0;
  delete latex1;
  delete latex2;
  delete legend;
  delete can_self;
}//Painter::~Painter()

//////////

void Painter::Draw(TVirtualPad* pad)
{
  pad->cd();
  
  //Get max and min
  float time_max = TMath::MaxElement(gr_vset->GetN(), gr_vset->GetX());
  float vset_max = TMath::MaxElement(gr_vset->GetN(), gr_vset->GetY());
  float vmon_max = TMath::MaxElement(gr_vmon->GetN(), gr_vmon->GetY());
  float imon_max = TMath::MaxElement(gr_imon->GetN(), gr_imon->GetY());
  
  float x_max = time_max*1.1;
  float v_max = max(vset_max, vmon_max)*1.1;
  float i_max = 0.01;//(imon_max+0.1)*1.1;
  
  //scale graph for imon 
  float scale = v_max/i_max;
  int n_point = gr_imon->GetN();
  double* time = gr_imon->GetX();
  double* imon = gr_imon->GetY();
  for(int i=0; i<n_point; i++) gr_imon_scale->SetPoint(i, time[i], scale*imon[i]);
  
  //multi graph
  gr_vset->Draw("AP");
  gr_vmon->Draw("SAMEP");
  gr_imon_scale->Draw("SAMEP");
  
  //axis range
  gr_vset->GetXaxis()->SetRangeUser(0, x_max);
  gr_vset->GetYaxis()->SetRangeUser(0, v_max);
  
  pad->Update();
  
  //2nd axis
  axis->SetX1(pad->GetUxmax());
  axis->SetX2(pad->GetUxmax());
  axis->SetY1(0);
  axis->SetY2(v_max);
  axis->SetWmin(0);
  axis->SetWmax(i_max);
  axis->SetTitleOffset(1.4);
  axis->Draw();
  
  latex0->Draw();
  latex1->Draw();
  latex2->Draw();
  
  legend->Draw(); 

  pad->Update();
  
  return;
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
