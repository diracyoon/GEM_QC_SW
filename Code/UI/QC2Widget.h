#ifndef QC2WIDGET_H
#define QC2WIDGET_H

#include <QWidget>
#include <QIntValidator>
#include <QMessageBox>
#include <QProcess>
#include <QCloseEvent>

namespace Ui
{
  class QC2Widget;
}

class QC2Widget : public QWidget
{
  Q_OBJECT
public:
  QC2Widget(QWidget *parent = nullptr);
  ~QC2Widget();

private:
  Ui::QC2Widget *ui;

  QIntValidator *batch_number_validator;
  QIntValidator *foil_number_validator;
  QDoubleValidator *rh_validator;
  QDoubleValidator *temperature_validator;

  QString process;
  QString foil_type;
  int batch_number;
  int foil_number;
  double rh;
  double temperature;
  int channel;
  QString tester;

  QPushButton *button_close;

  QCloseEvent* close_event;

  QString path;
  QProcess *qc2;

  void closeEvent(QCloseEvent* event);
  QString Get_Last_Sentence();

public slots:
  void Launch_QC2();
  void Read_Inputs();
  void Read_Process_Output(QProcess *process);
};

#endif // QC2ITERACTIVE_H
