#include <iostream>

#include "QC2Widget.h"
#include "ui_QC2Widget.h"

using namespace std;

QC2Widget::QC2Widget(QWidget *parent) : QWidget(parent), ui(new Ui::QC2Widget)
{
  qc2 = NULL;

  ui->setupUi(this);

  batch_number_validator = new QIntValidator(this);
  batch_number_validator->setBottom(0);
  ui->edit_batch_number->setValidator(batch_number_validator);

  foil_number_validator = new QIntValidator(this);
  foil_number_validator->setBottom(0);
  ui->edit_foil_number->setValidator(foil_number_validator);

  rh_validator = new QDoubleValidator(this);
  rh_validator->setBottom(0);
  rh_validator->setTop(100);
  rh_validator->setDecimals(1);
  ui->edit_rh->setValidator(rh_validator);

  temperature_validator = new QDoubleValidator(this);
  temperature_validator->setBottom(0);
  temperature_validator->setTop(100);
  temperature_validator->setDecimals(1);
  ui->edit_temperature->setValidator(temperature_validator);

  connect(ui->button_proceed, SIGNAL(clicked(bool)), this, SLOT(Read_Inputs()));
}

//////////

QC2Widget::~QC2Widget()
{
  delete ui;
  delete batch_number_validator;
  delete foil_number_validator;
  delete rh_validator;
  delete temperature_validator;
}

//////////

void QC2Widget::closeEvent(QCloseEvent* event)
{
 if(qc2!=NULL)
   {

     //running
     if(qc2->state()==QProcess::Running)
       {
         QString message = "QC process is still running. If you close the widget, the process will be terminated. Are you sure to close?";
         QMessageBox::StandardButton button_close = QMessageBox::question(this, "Confirm", "<font size = 5>"+message+"<\font>", QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);

         if(button_close == QMessageBox::Yes)
           {
              qc2->close();
              event->accept();

              return;
           }
         else
           {
                event->ignore();

                return;
           }
       }

     //finished
     else event->accept();

     return;
   }

 event->accept();
 return;
}

//////////

QString QC2Widget::Get_Last_Sentence()
{
  QTextCursor text_cursor = ui->text->textCursor();
  text_cursor.movePosition(QTextCursor::End);
  text_cursor.select(QTextCursor::LineUnderCursor);
  QString last_sentence = text_cursor.selectedText();

  return last_sentence;
}

//////////

void QC2Widget::Launch_QC2()
{
  QString foil_id = foil_type + "_" + QString("%1").arg(batch_number, 3, 10, QChar('0')) + "_";
  foil_id += QString("%1").arg(foil_number, 3, 10, QChar('0'));

  QString title = "Ch" + QString::number(channel) + "_" + process + "_" + foil_id;

  this->setWindowTitle(title);
  //disable proceed buttion
  ui->button_proceed->setEnabled(false);

  path = getenv("QC_SW_PATH");

  //check server is running
  ui->text->append("Check server is running.");

  //check channel is free or not
  ui->text->append("Check the channel is free.");

  QString target = path + "/Macro/Channel_Checker.sh";
  QString arg = QString::number(channel);

  QProcess* channel_checker = new QProcess;
  connect(channel_checker, &QProcess::readyRead, [=](){ this->Read_Process_Output(channel_checker); });

  channel_checker->start(target, QStringList() << arg);
  channel_checker->waitForFinished();

  QString last_sentence = Get_Last_Sentence();
  if(last_sentence.contains("Error")) return;
  else ui->text->append("The channel is free. Let's proceed.");

  delete channel_checker;

  //remove FIFO opened previously
  target = "rm " + path + "/FIFOs/FIFO" + QString::number(channel);
  QProcess rm_fifo;
  rm_fifo.start("/bin/bash", QStringList() << "-c" << target);
  rm_fifo.waitForFinished();

  //set trial number
  ui->text->append("Search trial records to allocate trial number.");

  target = path + "/Macro/Trial_Number_Setter.sh";

  QProcess *trial_number_setter = new QProcess;
  connect(trial_number_setter, &QProcess::readyRead, [=](){ this->Read_Process_Output(trial_number_setter); });

  trial_number_setter->start(target, QStringList() << process << foil_id);
  trial_number_setter->waitForFinished();

  delete trial_number_setter;

  last_sentence = Get_Last_Sentence();
  int trial_number = last_sentence.mid(last_sentence.lastIndexOf("=")+1, -1).toInt();

  target = path + "/Bin/" + process;

  //start QC2
  qc2 = new QProcess;
  connect(qc2, &QProcess::readyRead, [=](){ this->Read_Process_Output(qc2); });

  qc2->start(target, QStringList() << foil_id << QString::number(trial_number) << QString::number(channel) << QString::number(rh) << QString::number(temperature) << tester);
  qc2->waitForStarted();

  return;
}

//////////

void QC2Widget::Read_Process_Output(QProcess *process)
{
  QString str = process->readAll().trimmed();
  ui->text->append(str);

  return;
}

//////////

void QC2Widget::Read_Inputs()
{
  //process
  process = ui->combo_process->currentText();

  //foil type
  if(ui->button_GE21_M2->isChecked()) foil_type = "GE21M2";
  else if(ui->button_GE21_M3->isChecked()) foil_type = "GE21M3";
  else if(ui->button_GE21_M6->isChecked()) foil_type = "GE21M6";
  else if(ui->button_GE21_M7->isChecked()) foil_type = "GE21M7";
  else if(ui->button_ME0->isChecked()) foil_type = "ME0";

  //batch
  batch_number = ui->edit_batch_number->text().toInt();

  //foil number
  foil_number = ui->edit_foil_number->text().toInt();

  //rh
  rh = ui->edit_rh->text().toDouble();

  //temperature
  temperature = ui->edit_temperature->text().toDouble();

  //channel
  channel = ui->combo_channel_number->currentText().toInt();

  //tester
  tester = ui->edit_tester->text();

  QString message = "Please confirm your inputs are all right.<br>";
  message += "Foil type: " + foil_type + "<br>";
  message += "Batch#: " + QString::number(batch_number) + "<br>";
  message += "Foil#: " + QString::number(foil_number) + "<br>";
  message += "RH: " + QString::number(rh) + "<br>";
  message += "Temperature: " + QString::number(temperature) + "<br>";
  message += "Channel: " + QString::number(channel) + "<br>";
  message += "Tester: " + tester;

  QMessageBox::StandardButton button_proceed = QMessageBox::question(this, "Confirm", "<font size = 5>"+message+"<\font>", QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
  if(button_proceed==QMessageBox::Yes) Launch_QC2();

  return;
}

//////////
