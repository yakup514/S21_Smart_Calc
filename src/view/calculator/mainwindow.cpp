#include "mainwindow.h"
// #include "ui_credit_calc.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  credit_calc = new CreditCalc(this);
  def_calc = new DefaultCalc(this);
  debit_calc = new DebitCalc(this);

  connect(ui->actioncredit, SIGNAL(triggered()), this,
          SLOT(CreditCalcChoose()));
  connect(ui->actiondefault, SIGNAL(triggered()), this,
          SLOT(DefaultCalcChoose()));
  connect(ui->actiondebit, SIGNAL(triggered()), this, SLOT(DebitCalcChoose()));
  this->setFixedSize(420, 420);
  if (!debit_calc->isHidden()) {
    debit_calc->hide();
  }
  if (!credit_calc->isHidden()) {
    credit_calc->hide();
  }
  def_calc->show();

  def_calc->is_show = true;
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::CreditCalcChoose() {
  if (def_calc->is_show) {
    def_calc->hide();
    def_calc->is_show = false;
  }
  if (!debit_calc->isHidden()) {
    debit_calc->hide();
  }

  this->setMinimumSize(500, 300);
  this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  credit_calc->show();

  credit_calc->is_show = true;
}

void MainWindow::DefaultCalcChoose() {
  if (credit_calc->is_show) {
    credit_calc->hide();
    credit_calc->is_show = false;
  }
  if (!debit_calc->isHidden()) {
    debit_calc->hide();
  }
  this->setFixedSize(420, 420);

  def_calc->show();
  def_calc->is_show = true;
}

void MainWindow::DebitCalcChoose() {
  if (!credit_calc->isHidden()) {
    credit_calc->hide();
  }
  if (!def_calc->isHidden()) {
    def_calc->hide();
  }
  this->setMinimumSize(700, 770);
  this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
  setCentralWidget(debit_calc);

  debit_calc->show();
}
