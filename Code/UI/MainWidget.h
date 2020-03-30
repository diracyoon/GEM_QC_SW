#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QProcess>

#include "QC2Widget.h"
#include "QC2Widget.h"

namespace Ui
{
  class MainWidget;
}

class MainWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MainWidget(const QString& a_mode = "NORMAL", QWidget *parent = 0);
  ~MainWidget();

private:
  Ui::MainWidget *ui;

  QString path;
  QString mode;
  QString tty;

  QProcess *server;
  QProcess *monitor;

  void closeEvent(QCloseEvent *event);
  QString Get_Last_Sentence();

public slots:
  void Close_Monitor();
  void Launch_Monitor();
  void Launch_QC2();
  void Read_Process_Output(QProcess *process);
};

#endif // MAINWIDGET_H
