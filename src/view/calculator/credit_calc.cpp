#include "credit_calc.h"

#include <math.h>

#include <QDateTime>
#include <QDoubleValidator>
#include <QIntValidator>

#include "ui_credit_calc.h"

CreditCalc::CreditCalc(QWidget* parent)
    : QWidget(parent), ui(new Ui::CreditCalc) {
  ui->setupUi(this);
  connect(ui->pushButton, SIGNAL(clicked()), this,
          SLOT(SlotCalcButtonClicked()));
  QDoubleValidator* double_val = new QDoubleValidator(0, 1000000000.0, 2, this);
  QIntValidator* int_val = new QIntValidator(1, 100, this);
  QDoubleValidator* perc_val = new QDoubleValidator(0.01, 999.0, 2, this);
  double_val->setLocale(QLocale::English);
  int_val->setLocale(QLocale::English);
  perc_val->setLocale(QLocale::English);
  ui->line_sum->setValidator(double_val);
  ui->line_time->setValidator(int_val);
  ui->line_per->setValidator(perc_val);
  ui->line_sum->setText("1000");
  ui->line_per->setText("10");
  ui->line_time->setText("5");
  ui->ann_checkbox->setCheckState(Qt::Checked);
}

CreditCalc::~CreditCalc() { delete ui; }

void CreditCalc::SlotCalcButtonClicked() {
  bool ok1, ok2, ok3;
  double sum = ui->line_sum->text().toDouble(&ok1);
  int time = ui->line_time->text().toInt(&ok2);
  double per = ui->line_per->text().toDouble(&ok3);
  // qDebug() << sum << " " << time << " " << per;
  if (ui->comb_year_mont->currentText() == "лет") time *= 12;

  if (ok1 && ok2 && ok3 && time > 0 && per >= 0.01) {
    if (ui->ann_checkbox->isChecked()) {
      per /= 1200;

      double kmonth_pay =
          per * pow(1.0 + per, time) / (pow(1.0 + per, time) - 1.0);
      double month_pay = round(sum * kmonth_pay * 100) / 100;
      ui->month_pay_label->setText(QString::number(month_pay, 'f', 2) + "₽");
      double pay_per = month_pay * time;
      ui->proc_label->setText(QString::number(pay_per - sum, 'f', 2) + "₽");
      ui->pay_label->setText(QString::number(pay_per, 'f', 2) + "₽");
    } else {
      double month_pay = sum / time;
      double temp = sum;
      double first_month =
          month_pay + round((sum * per / 100 / 12) * 100) / 100;
      double sum_payd = first_month;

      double last_month = 0;
      for (int i = 1; i < time; ++i) {
        sum -= month_pay;
        last_month = round((month_pay + ((sum)*per / 100 / 12)) * 100) / 100;

        sum_payd = round((sum_payd + last_month) * 100) / 100;
      }
      ui->month_pay_label->setText(QString::number(first_month, 'f', 2) +
                                   " ... " + QString::number(last_month) + "₽");
      ui->proc_label->setText(QString::number(sum_payd - temp) + "₽");
      ui->pay_label->setText(QString::number(sum_payd) + "₽");
    }
  }
}
