#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "credit_calc.h"
#include "debit_calc.h"
#include "default_calc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 public slots:
  void CreditCalcChoose();
  void DebitCalcChoose();
  void DefaultCalcChoose();

 private:
  Ui::MainWindow* ui;
  DefaultCalc* def_calc;
  CreditCalc* credit_calc;
  DebitCalc* debit_calc;
};

#endif  // MAINWINDOW_H
