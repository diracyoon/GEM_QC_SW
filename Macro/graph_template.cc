#include <map>
#include <TLegend.h>
#include <TCanvas.h>
#include <typeinfo>
#include <string>
#include <TColor.h>
#include <TString.h>
#include <TLine.h>
#include <iostream>

using namespace std;

//variables
TString filename = "InputFileName";    // Input .result file that modified by run.sh
int run_number, ch_number;             
TString foilname, str1, str2;
string border = "################################";
int border_number = 0;                    
TString xaxis = "Time (s)";
TString y1axis = "Voltage (V)";
TString y2axis = "Current (#muA)"; 
int num_set = 20000;                   // Number of data points
int line_number = 0, entry_number = 0;
double xmin = 0., xmax = 0., y1min = 0., y1max = 40., y2min = 0., y2max = 80.;    // y1 = left-side y axis, y2 = right-side y axis
double scale = y1max/y2max;            // For matching to the y2 axis scale 
//int k = 0;

void graph_template(){

  double num1[num_set], num2[num_set], num3[num_set], num4[num_set];

  ifstream infile;
  infile.open(filename);                // Open .result file
  string line;
  if(!infile.is_open()){
    cout << filename + " does not exist!!" << endl;
    return;    
  }

  while(!infile.eof()){
    getline(infile, line);              // Get lines from .result file
    if(infile.eof()) break;
    line_number++;
    int cmp = border.compare(1,3,line,1,3);            // Compare #### & line. If they are same, cmp is 0.
    if(cmp == 0) border_number++;                           
    if(line_number == 1){ istringstream istr(line); istr >> str1 >> run_number; }                   // Get the run number
    if(line_number == 2){ istringstream istr(line); istr >> str1 >> str2 >> foilname; }             // Get the foil name
    if(line_number == 3){ istringstream istr(line); istr >> str2 >> ch_number; }                    // Get the channel number
    if((border_number == 2) && (cmp != 0)){                 // Data lines are between 2nd #### & 3rd ####.
    
      istringstream istr(line);
      if(cmp == 0) break;
      entry_number++;
      istr >> num1[entry_number] >> num2[entry_number] >> num3[entry_number] >> num4[entry_number];
      num4[entry_number] = scale*num4[entry_number];     // For matching to the y2 axis scale
    }
  }
  infile.close();

  xmax = num1[entry_number] + 10.;                       // num1[entry_number] is the running time of QC2 Long.
  cout << "Run = " << run_number << ", Foil = " << foilname << ", Channel = " << ch_number << endl;

  TCanvas *c1 = new TCanvas("c1", "", 1200, 1000);

  TMultiGraph *mg = new TMultiGraph();
  TGraph *gr21 = new TGraph(entry_number, num1, num2);
  TGraph *gr31 = new TGraph(entry_number, num1, num3);
  TGraph *gr41 = new TGraph(entry_number, num1, num4);

  c1->cd();

  mg->Add(gr21);
  mg->Add(gr31);
  mg->Add(gr41);
  mg->Draw("AP");

  TString title = Form("Run = %d, Foil = %s , Channel = %d", run_number, foilname.Data(), ch_number);
  mg->SetTitle(title);
  mg->GetXaxis()->SetTitle(xaxis);
  mg->GetXaxis()->SetTitleSize(0.04);
  mg->GetXaxis()->SetLabelSize(0.04);
  mg->GetYaxis()->SetTitle(y1axis);
  mg->GetYaxis()->SetTitleSize(0.04);
  mg->GetYaxis()->SetLabelSize(0.04);

  TLegend *legend = new TLegend(.77,.77,.87,.87);
  legend->AddEntry(gr21, "V_{set}", "l");
  legend->AddEntry(gr31, "V_{mon}", "l");
  legend->AddEntry(gr41, "I_{mon}", "l");
  legend->SetLineWidth(0);
  legend->Draw();

  gPad->Modified();
  mg->SetMinimum(y1min);
  mg->SetMaximum(y1max);
  mg->GetXaxis()->SetLimits(xmin, xmax);

  TGaxis *axis = new TGaxis(xmax, y1min, xmax, y1max, y2min, y2max, 510, "+L"); // y2 axis
  axis->SetTitle(y2axis);
  axis->SetTitleSize(0.04);
  axis->SetTitleFont(42);
  axis->SetLabelSize(0.04);
  axis->SetLabelFont(42);
  axis->Draw();

  // Monitoring part
  while(border_number == 2){
//  while(k < 10){                                // Just to check the script runs well
    gSystem->Sleep(1000);                         // Time interval for the update. 1000 = 1 second.
    *gr21 = TGraph(entry_number, num1, num2);
    *gr31 = TGraph(entry_number, num1, num3);
    *gr41 = TGraph(entry_number, num1, num4);

    gr21->SetMarkerStyle(20);
    gr21->SetMarkerColor(kRed);
    gr21->SetLineColor(kRed);
    gr21->SetLineWidth(2);

    gr31->SetMarkerStyle(20);
    gr31->SetMarkerColor(kOrange);
    gr31->SetLineColor(kOrange);
    gr31->SetLineWidth(2);

    gr41->SetMarkerStyle(20);
    gr41->SetMarkerColor(kBlue);
    gr41->SetLineColor(kBlue);
    gr41->SetLineWidth(2);

//    TString title2 = Form("time : %d (s)", k);
//    axis->SetTitle(title2);

    c1->Modified();
    c1->Update();
    if(gSystem->ProcessEvents()) break;
//    k++;
  }
  
  cout << "QC2 Long has been finished!" << endl;
  TString output = Form("%d_%s.png", run_number, foilname.Data());
  c1->SaveAs(output);
}
