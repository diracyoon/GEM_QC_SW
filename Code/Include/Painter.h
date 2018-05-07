#include <iostream>
#include <fstream>
#include <sstream>

#include <TVirtualPad.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TAxis.h>

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

  int runnumber;
  string foil_name;
  int channel;
  float rh;
  float temperature;
  string tester;
  
  string file_path;
  ifstream fin;  
 
  float xmin;
  float xmax;
  float y1min;
  float y1max;
  float y2min;
  float y2max;

  TString title;
  TString text1;
  TString text2;

  TMultiGraph* gr; 
  TGraphErrors* gr_vset;
  TGraphErrors* gr_vmon;
  TGraphErrors* gr_imon;
  TGaxis* axis;
  TLegend* legend;
  TLatex* latex1;
  TLatex* latex2;
};
