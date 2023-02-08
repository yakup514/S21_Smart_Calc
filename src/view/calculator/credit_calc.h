#ifndef CREDIT_CALC_H
#define CREDIT_CALC_H

#include <QWidget>

namespace Ui {
class CreditCalc;
}

class CreditCalc : public QWidget {
  Q_OBJECT

 public:
  bool is_show;
  explicit CreditCalc(QWidget *parent = nullptr);
  ~CreditCalc();

 public:
  Ui::CreditCalc *ui;

 public slots:
  void SlotCalcButtonClicked();
};

#endif  // CREDIT_CALC_H
