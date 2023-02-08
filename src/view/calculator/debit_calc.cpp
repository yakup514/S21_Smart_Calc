#include "debit_calc.h"

#include <QDate>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include <iterator>

#include "ui_debit_calc.h"
DebitCalc::DebitCalc(QWidget* parent) : QWidget(parent), ui(new Ui::DebitCalc) {
  ui->setupUi(this);
  QDoubleValidator* double_vall =
      new QDoubleValidator(0, 1000000000.0, 2, this);
  QIntValidator* int_vall = new QIntValidator(1, 100, this);
  QDoubleValidator* perc_vall = new QDoubleValidator(0.01, 999.0, 2, this);
  double_vall->setLocale(QLocale::English);
  int_vall->setLocale(QLocale::English);
  perc_vall->setLocale(QLocale::English);
  ui->line_sum->setValidator(double_vall);
  ui->line_time->setValidator(int_vall);
  ui->line_per->setValidator(perc_vall);
  ui->line_add->setValidator(double_vall);
  ui->line_time->setText("12");
  ui->line_sum->setText("100000");
  ui->line_per->setText("12");
  ui->line_key_rate->setText("7.5");
  ui->line_key_rate->setValidator(perc_vall);

  QDate date;
  ui->date_edit_begin->setDate(date.currentDate());
  ui->date_edit_add->setDate(date.currentDate());
  connect(ui->calc_btn, SIGNAL(clicked()), this, SLOT(CalculateBtnClicked()));
  connect(ui->add_btn, SIGNAL(clicked()), this, SLOT(AddBtnClicked()));
  connect(ui->del_button, SIGNAL(clicked()), this, SLOT(DelBtnClicked()));
  credit_model = new QStandardItemModel(this);
  debit_model = new QStandardItemModel(this);
  final_model = new QStandardItemModel(this);
  QStringList labels = {"дата", "сумма"};
  credit_model->setHorizontalHeaderLabels(labels);
  debit_model->setHorizontalHeaderLabels(labels);
  ui->credit_table_view->setModel(credit_model);
  ui->debit_table_view->setModel(debit_model);
  QStringList labels2 = {"   Дата   ", "   Начислено процентов,₽   ",
                         "   Вклад пополнен,₽   ", "   Остаток на вкладе,₽   "};
  final_model->setHorizontalHeaderLabels(labels2);
  ui->final_table_view->setModel(final_model);
  ui->final_table_view->resizeColumnsToContents();
  ui->credit_table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->debit_table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->final_table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

DebitCalc::~DebitCalc() {
  delete ui;

  // debits_del_btns.clear();
}

void DebitCalc::DelBtnClicked() {
  debits.clear();
  credit_model->clear();
  debit_model->clear();
  QStringList labels = {"дата", "сумма"};
  credit_model->setHorizontalHeaderLabels(labels);
  debit_model->setHorizontalHeaderLabels(labels);
}

void DebitCalc::AddBtnClicked() {
  QDate date = ui->date_edit_add->date();
  double val = ui->line_add->text().toDouble();
  // debits[date] += val;

  if (ui->credit_debit_combo_box->currentIndex() == 1) {
    QStandardItem* item = new QStandardItem(date.toString("dd-MM-yy"));
    credit_model->setItem(credit_model->rowCount(), 0, item);
    item = new QStandardItem(QString::number(val));
    credit_model->setItem(credit_model->rowCount() - 1, 1, item);
    debits[date] -= val;
  } else {
    QStandardItem* item = new QStandardItem(date.toString("dd-MM-yy"));
    debit_model->setItem(debit_model->rowCount(), 0, item);
    item = new QStandardItem(QString::number(val));
    debit_model->setItem(debit_model->rowCount() - 1, 1, item);
    debits[date] += val;
  }
}

void DebitCalc::FillModelRow(QString date, QString per, QString add,
                             QString sum) {
  QStandardItem* item = new QStandardItem(date);
  final_model->setItem(final_model->rowCount(), 0, item);
  if (!per.isEmpty()) {
    item = new QStandardItem(per);
    final_model->setItem(final_model->rowCount() - 1, 1, item);
  }
  if (!add.isEmpty()) {
    item = new QStandardItem(add);
    final_model->setItem(final_model->rowCount() - 1, 2, item);
  }
  if (!sum.isEmpty()) {
    item = new QStandardItem(sum);
    final_model->setItem(final_model->rowCount() - 1, 3, item);
  }
  ui->final_table_view->resizeColumnsToContents();
}

double DebitCalc::PercCalculate(QDate start, QDate finish, double sum,
                                int proc) {
  double result = 0;
  double diy1 = start.daysInYear() * 1.0;
  double diy2 = finish.daysInYear() * 1.0;
  QDate end_of_m;
  int per = 0;
  end_of_m.setDate(start.year(), start.month(), start.daysInMonth());
  if (finish <= end_of_m) {
    per = start.daysTo(finish);
    result += per / diy1 * proc / 100.0 * sum;
  } else {
    per = start.daysTo(end_of_m);
    result += per / diy1 * proc / 100.0 * sum;
    per = end_of_m.daysTo(finish);
    result += per / diy2 * proc / 100.0 * sum;
  }
  return result;
}

void DebitCalc::CalculateBtnClicked() {
  if (!ui->line_time->text().isEmpty() && !ui->line_sum->text().isEmpty()) {
    final_model->clear();
    QStringList labels2 = {"   Дата   ", "   Начислено процентов,₽   ",
                           "   Вклад пополнен,₽   ",
                           "   Остаток на вкладе,₽   "};
    final_model->setHorizontalHeaderLabels(labels2);
    ui->final_table_view->setModel(final_model);
    // ui->final_table_view->resizeColumnsToContents();
    std::map<QDate, double>::iterator it = debits.begin();
    int time = ui->line_time->text().toInt();
    QDate start_period = ui->date_edit_begin->date();
    QDate end_period = ui->date_combo_box->currentIndex() == 0
                           ? start_period.addMonths(time)
                           : start_period.addMonths(time * 12);
    double sum = ui->line_sum->text().toDouble();
    double procent = ui->line_per->text().toDouble();
    double final_proc_sum = 0.0;
    int period = 0;
    if (ui->period_combo_box->currentIndex() == 0)
      period = 1;
    else if (ui->period_combo_box->currentIndex() == 1)
      period = 12;
    else
      period = time;
    FillModelRow(start_period.toString("dd-MM-yy"), "",
                 QString::number(sum, 'f', 2), QString::number(sum, 'f', 2));

    while (start_period < end_period) {
      double proc_sum = 0.0;
      QDate temp_date = start_period;
      it = debits.upper_bound(start_period);
      start_period = start_period.addMonths(period);

      while (it != debits.end() && it->first <= start_period) {
        //                if (it->second < 0 && fabs(it->second) > sum) {
        //                    temp_date = it->first;
        //                    it = debits.upper_bound(temp_date);
        //                    qDebug()<<sum<<"   " << it->first;
        //                   continue;
        //                }
        proc_sum += PercCalculate(temp_date, it->first, sum, procent);
        temp_date = it->first;
        if (it->second < 0 && fabs(it->second) > sum) {
          sum += 0;
        } else {
          sum += it->second;
          FillModelRow(it->first.toString("dd-MM-yy"), "",
                       QString::number(it->second, 'f', 2),
                       QString::number(sum, 'f', 2));
        }
        it = debits.upper_bound(temp_date);
      }

      proc_sum += PercCalculate(temp_date, start_period, sum, procent);
      if (ui->kap_check_box->isChecked()) {
        sum += proc_sum;
        FillModelRow(start_period.toString("dd-MM-yy"),
                     QString::number(proc_sum, 'f', 2),
                     QString::number(proc_sum, 'f', 2),
                     QString::number(sum, 'f', 2));
      } else {
        FillModelRow(start_period.toString("dd-MM-yy"),
                     QString::number(proc_sum, 'f', 2), "0.00",
                     QString::number(sum, 'f', 2));
      }
      final_proc_sum += proc_sum;
    }
    FillModelRow("Итого:",
                 QString::number(final_proc_sum, 'f', 2) + "\n" +
                     "(сумма начисленных проентов)",
                 "",
                 QString::number(sum, 'f', 2) + "\n" + "(остаток на вкладе)");
    ui->final_table_view->resizeRowsToContents();
    double not_tax = ui->line_key_rate->text().toDouble() * 1000000.0 / 100;
    double tax =
        final_proc_sum > not_tax ? (final_proc_sum - not_tax) * 0.13 : 0.0;
    ui->lab_proc->setText(QString::number(final_proc_sum, 'f', 2));
    ui->lab_sum->setText(QString::number(sum, 'f', 2));
    ui->lab_tax->setText(QString::number(tax, 'f', 2));
  }
}
