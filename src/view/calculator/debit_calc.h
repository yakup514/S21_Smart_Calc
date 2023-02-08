#ifndef DEBIT_CALC_H
#define DEBIT_CALC_H
#include <QLineEdit>
#include <QPushButton>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QWidget>

namespace Ui {
class DebitCalc;
}

class DebitCalc : public QWidget {
  Q_OBJECT

 public:
  explicit DebitCalc(QWidget* parent = nullptr);

  ~DebitCalc();

 private:
  Ui::DebitCalc* ui;
  QStandardItemModel* debit_model;
  QStandardItemModel* credit_model;
  QStandardItemModel* final_model;
  std::map<QDate, double> debits;
  void FillModelRow(QString date, QString per, QString add, QString sum);
  double PercCalculate(QDate start, QDate finish, double sum, int proc);

 public slots:
  void CalculateBtnClicked();
  void AddBtnClicked();
  void DelBtnClicked();
};

#endif  // DEBIT_CALC_H
