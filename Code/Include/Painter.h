#include <iostream>
#include <fstream>
#include <sstream>

#include <TVirtualPad.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TLatex.h>
#include <TGaxis.h>
#include <TLegend.h>
#include <TMath.h>

using namespace std;

class Painter
{
 public:
  Painter(const string& a_file_path);
  ~Painter();

  void Draw(TVirtualPad* pad);
  void Update();
    
 protected:
  TVirtualPad* pad;

  string mode;
  string process;
  string foil_name;
  int trial_number;
  int channel;
  float rh;
  float temperature;
  string tester;
  
  string file_path;
  ifstream fin;
  
  TGraphErrors* gr_vset;
  TGraphErrors* gr_vmon;
  TGraphErrors* gr_imon;
  TGraphErrors* gr_imon_scale;
  
  TGaxis* axis;
  
  TLatex* latex0;
  TLatex* latex1;
  TLatex* latex2;
  
  TLegend* legend;
};
