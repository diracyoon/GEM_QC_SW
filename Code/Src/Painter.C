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

  //multigraph
  gr = new TMultiGraph(); 
  gr_vset = new TGraphErrors();
  gr_vmon = new TGraphErrors();
  gr_imon = new TGraphErrors();
  title = Form("Run : %d, Foil : %s , Channel : %d", runnumber, foil_name.Data(), channel);

  //2nd yaxis
  axis = new TGaxis(xmax, y1min, xmax, y1max, y2min, y2max, 510, "+L");

  //addtional text
  text1 = Form("Humidity : %f%", rh);
  text2 = Form("Temperature : %f#circC", temperature);

  latex1 = new TLatex(.13, .87, text1);
  latex2 = new TLatex(.13, .82, text2);

  //legend
  legend = new TLegend(.77, .75, .87, .87);

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
 
  delete gr; 
  delete gr_vset;
  delete gr_vmon;
  delete gr_imon;
  delete can_self;
  delete axis;
  delete latex1;
  delete latex2;
  delete legend;
}//Painter::~Painter()

//////////

void Painter::Draw(TVirtualPad* pad)
{
  pad->cd();

  gr->Add(gr_vset);
  gr->Add(gr_vmon);
  gr->Add(gr_imon);
  gr->Draw("AP*");

  gr->SetTitle(title);
  gr->GetXaxis()->SetTitle("Time (s)");
  gr->GetYaxis()->SetTitle("Voltage (V)");
  gr->GetXaxis()->SetTitleSize(0.04);
  gr->GetXaxis()->SetLabelSize(0.04);
  gr->GetYaxis()->SetTitleSize(0.04);
  gr->GetYaxis()->SetLabelSize(0.04);

  float timemax = TMath::MaxElement(gr_vset->GetN(), gr_vset->GetX());
  float vsetmax = TMath::MaxElement(gr_vset->GetN(), gr_vset->GetY());
  float imonmax = TMath::MinElement(gr_imon->GetN(), gr_imon->GetY());

  xmin = 0.; xmax = timemax + 10.;
  y1min = 0.; y1max = vsetmax + 10.;
  y2min = 0.; y2max = imonmax + 10.;

  gr->SetMinimum(y1min);
  gr->SetMaximum(y1max);
  gr->GetXaxis()->SetLimits(xmin, xmax); 
  gr->SetMinimum(y1min);
  gr->SetMaximum(y1max);
  gr->GetXaxis()->SetLimits(xmin, xmax); 
 
  axis->SetTitle("Current (#muA)");
  axis->SetTitleSize(0.04);
  axis->SetTitleFont(42);
  axis->SetLabelSize(0.04);
  axis->SetLabelFont(42);
  axis->Draw();

  latex1->SetNDC();
  latex1->SetTextSize(0.04);
  latex1->SetTextAlign(12);
  latex1->SetTextFont(42);
  latex1->Draw();

  latex2->SetNDC();
  latex2->SetTextSize(0.04);
  latex2->SetTextAlign(12);
  latex2->SetTextFont(42);
  latex2->Draw();

  legend->AddEntry(gr_vset, "V_{set}", "l");
  legend->AddEntry(gr_vmon, "V_{mon}", "l");
  legend->AddEntry(gr_imon, "I_{mon}", "l");
  legend->SetLineWidth(0);
  legend->Draw(); 

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

      //match the curret(I_set) scale to the voltage scale
      float vsetmax = TMath::MaxElement(gr_vset->GetN(),gr_vset->GetY());
      float imonmax = TMath::MaxElement(gr_imon->GetN(),gr_imon->GetY());
      float scale = (vsetmax + 10.)/(imonmax + 10.);
      imon = scale*imon;
      gr_imon->SetPoint(n_point, time, imon);

      //graph style
      gr_vset->SetMarkerStyle(20);
      gr_vset->SetMarkerColor(kRed);
      gr_vset->SetLineColor(kRed);
      gr_vset->SetLineWidth(2);

      gr_vmon->SetMarkerStyle(20);
      gr_vmon->SetMarkerColor(kOrange);
      gr_vmon->SetLineColor(kOrange);
      gr_vmon->SetLineWidth(2);

      gr_imon->SetMarkerStyle(20);
      gr_imon->SetMarkerColor(kBlue);
      gr_imon->SetLineColor(kBlue);
      gr_imon->SetLineWidth(2);
    }
  
  return;
}//void Painter::Update()
  
///////////
