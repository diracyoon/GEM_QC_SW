#include <iostream>

#include "MainWidget.h"
#include "ui_MainWidget.h"

using namespace std;

//////////

MainWidget::MainWidget(const QString &a_mode, QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget)
{
  server = NULL;
  monitor = NULL;

  ui->setupUi(this);

  connect(ui->button_qc2, SIGNAL(clicked(bool)), SLOT(Launch_QC2()));
  connect(ui->button_monitor, SIGNAL(clicked()), this, SLOT(Launch_Monitor()));
  connect(ui->button_close_monitor, SIGNAL(clicked(bool)), this, SLOT(Close_Monitor()));

  path = getenv("QC_SW_PATH");

  mode = a_mode;
  if(mode=="DEBUG")
   {
     ui->text->append("DEBUG mode set");
     
     ui->mode_indicator->setChecked(1);
     ui->mode_indicator->setEnabled(0);

     tty = "test_port";
   }
 else
   {
     ui->text->append("NORMAL mode set");
     
     ui->mode_indicator->setChecked(0);
     ui->mode_indicator->setEnabled(0);

     //find device tty
     ui->text->append("Finding device tty");
     
     QProcess* tty_finder = new QProcess;
     connect(tty_finder, &QProcess::readyRead, [=](){ this->Read_Process_Output(tty_finder); });

     QString target = path + "/Macro/TTY_Finder.sh";

     tty_finder->start(target, QStringList());
     tty_finder->waitForFinished();

     delete tty_finder;

     QString sentence = Get_Last_Sentence();

     if(sentence.contains("Error")) return;
     else tty = sentence;
   }
  ui->text->append("TTY = " + tty);
  
  ui->chk_submit->setChecked(1);

  //remove fifo opened previously
  ui->text->append("Remove FIFO opened previously");

  QString target = "rm " + path + "/FIFOs/FIFO";
  QProcess rm_fifo;
  rm_fifo.start("/bin/bash", QStringList() << "-c" << target);
  rm_fifo.waitForFinished();

 //start server
 ui->text->append("Start Server");

 target = path + "/Bin/Server";

 QStringList arg;
 arg << tty << mode;

 server = new QProcess;
 connect(server, &QProcess::readyRead, [=](){ this->Read_Process_Output(server); });

 server->start(target, arg);
 server->waitForStarted();
}

//////////

MainWidget::~MainWidget()
{
  delete ui;
  //delete server;
  //delete monitor;
}

//////////

void MainWidget::closeEvent(QCloseEvent *event)
{
  if(server!=NULL) server->close();
  if(monitor!=NULL) monitor->close();

  event->accept();

  return;
}

//////////

QString MainWidget::Get_Last_Sentence()
{
  QTextCursor text_cursor = ui->text->textCursor();
  text_cursor.movePosition(QTextCursor::End);
  text_cursor.select(QTextCursor::LineUnderCursor);
  QString last_sentence = text_cursor.selectedText();

  return last_sentence;
}

//////////

void MainWidget::Close_Monitor()
{
  if(monitor==NULL)
    {
      ui->text->append("No monitor is running");
    }
  else
    {
      ui->text->append("Close Monitor");
      monitor->close();
      monitor = NULL;
    }

  return;
}

//////////

void MainWidget::Launch_Monitor()
{
  ui->text->append("Start Monitor");

  QString target = path + "/Bin/Monitor";

  QStringList arg;
  bool chk_submit = ui->chk_submit->isChecked();
  if(chk_submit) arg << "TRUE";
  else arg << "FALSE";

  monitor = new QProcess();
  connect(monitor, &QProcess::readyRead, [=](){ this->Read_Process_Output(monitor); });

  monitor->start(target, arg);

  return;
}

//////////

void MainWidget::Launch_QC2()
{
  QC2Widget *qc2widget = new QC2Widget();
  qc2widget->show();

  return;
}

//////////

void MainWidget::Read_Process_Output(QProcess *process)
{
  QString str = process->readAll().trimmed();
  ui->text->append(str);

  return;
}

//////////
