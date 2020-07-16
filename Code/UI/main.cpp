#include <iostream>

#include <QApplication>

#include "MainWidget.h"

using namespace std;

int main(int argc, char *argv[])
{
  string mode;
  if(argc!=1&&argc!=2)
    {
      cerr << "Wrong number of argc." << endl;
      cerr << "argv[1]: mode i.e. NORMAL or DEBUG" << endl;
      exit(1);
    }

  if(argc==1) mode = "NORMAL";
  else if(argc==2) mode = argv[1];

  //mode = "DEBUG";

  QApplication a(argc, argv);
  a.setQuitOnLastWindowClosed(1);

  MainWidget w(mode.c_str());
  w.show();

  return a.exec();
}
