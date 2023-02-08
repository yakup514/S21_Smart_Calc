#include "default_calc.h"

#include <QRegularExpression>

#include "graph.h"
#include "ui_default_calc.h"
#include "utils.h"

DefaultCalc::DefaultCalc(QWidget* parent)
    : QWidget(parent), ui(new Ui::DefaultCalc) {
  // ui->setupUi(this);
  this->setFixedSize(420, 420);
  // ent_x_ = false;
  line1 = new QLineEdit;
  line1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  line2 = new QLineEdit;
  line2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  line2->setText("0");
  line1->setAlignment(Qt::AlignRight);
  line1->setReadOnly(true);
  line1->setObjectName("line1");
  line2->setObjectName("line2");
  line2->setReadOnly(true);
  line2->setAlignment(Qt::AlignRight);
  //    line1->setStyleSheet("border: none");
  //    line2->setStyleSheet("border: none");

  QString buttons[5][7] = {{"(", ")", "^", "AC", "±", "del", "/"},
                           {"cos", "acos", "sqrt", "7", "8", "9", "*"},
                           {"sin", "asin", "ln", "4", "5", "6", "-"},
                           {"tan", "atan", "log", "1", "2", "3", "+"},
                           {"graph", "mod", "x", "0", "", ".", "="}};

  QGridLayout* layout = new QGridLayout;
  layout->addWidget(line1, 0, 0, 1, 7);
  layout->addWidget(line2, 1, 0, 1, 7);
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 7; ++j) {
      if (buttons[i][j] != "graph" && buttons[i][j] != "0" &&
          !buttons[i][j].isEmpty()) {
        layout->addWidget(CreateButton(buttons[i][j]), i + 2, j);
      }
    }
  }
  layout->addWidget(CreateButton("graph"), 6, 0, 1, 1);
  layout->addWidget(CreateButton("0"), 6, 3, 1, 2);

  setLayout(layout);
  layout->setSpacing(0);

  layout->setContentsMargins(0, 0, 0, 0);
  // layout->setAlignment(Qt::AlignTop);
}

QPushButton* DefaultCalc::CreateButton(const QString& str) {
  QPushButton* button = new QPushButton(str);
  if (str == "+" || str == "-" || str == "*" || str == "/" || str == "=") {
    button->setObjectName("orange_button");
  } else if (str == "0" || str == "1" || str == "2" || str == "3" ||
             str == "4" || str == "5" || str == "6" || str == "7" ||
             str == "8" || str == "9" || str == ".") {
    button->setObjectName("numeric_button");
  }
  button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  if (str != "graph")
    connect(button, SIGNAL(clicked()), this, SLOT(SlotButtonClicked()));
  else
    connect(button, SIGNAL(clicked()), this, SLOT(SlotGraphClicked()));
  return button;
}


void DefaultCalc::SlotButtonClicked() {
  QString str = ((QPushButton*)sender())->text();
  if (status == STATUS_DEFAULT) {
    if (str == "AC") {
      data_.clear();
      x_value_.clear();
      line1->clear();
      line2->setText("0");
    }
    if (str == ".") {
      if (data_.isEmpty()) data_ = "0";
      data_ += str;
      line2->setText(data_);
    }
    if (str == "del" && !data_.isEmpty()) {
      data_.chop(1);
      line2->setText(data_);
    }
    if (str == "±" && !data_.isEmpty()) {
      Negate(data_);
      line2->setText(data_);
    } else if (str == "cos" || str == "sin" || str == "tan" || str == "acos" ||
               str == "asin" || str == "atan" || str == "sqrt" || str == "ln" ||
               str == "log") {
      data_ += str;
      data_ += "(";
      line2->setText(data_);
    } else if (str != "." && str != "=" && str != "±" && str != "del" &&
               str != "AC") {
      data_ += str;
      line2->setText(data_);
    } else if (str == "=" && !data_.isEmpty()) {
      line1->setText(data_ + "=");
      if (!data_.contains("x")) {
        double result;
        int err = Calculate_(result, data_);
        if (!err) {
          data_ = QString::number(result);
          line2->setText(data_);
          status = STATUS_RESULT;
        } else {
          data_.clear();
          line1->clear();
          line2->setText(kErr[err]);
        }
      } else {
        line2->setText("ENTER X AND PRESS =");
        status = STATUS_XVAL;
      }
    }
  } else if (status == STATUS_RESULT) {
    if (str == "±" || str == "AC" || str == "+" || str == "-" || str == "*" ||
        str == "/" || str == "mod" || str == "^" || str == "del") {
      status = STATUS_DEFAULT;
      line1->clear();
      SlotButtonClicked();
    } else if (str == "cos" || str == "sin" || str == "tan" || str == "acos" ||
               str == "asin" || str == "atan" || str == "sqrt" || str == "ln" ||
               str == "log") {
      status = STATUS_DEFAULT;
      data_ = str + "(" + data_;
      line1->clear();
      line2->setText(data_);
    } else if (str != "=" && str != ".") {
      status = STATUS_DEFAULT;
      data_ = str;
      line1->clear();
      line2->setText(data_);
    }
  } else if (status == STATUS_XVAL) {
    if (str == "AC") {
      x_value_.clear();
      line1->clear();
      line2->setText("0");
      data_.clear();
      status = STATUS_DEFAULT;
    }
    if (str == "." && !x_value_.contains(".")) {
      if (x_value_.isEmpty()) x_value_ = "0";
      x_value_ += str;
      line2->setText(x_value_);
    }
    if (str == "del" && !x_value_.isEmpty()) {
      x_value_.chop(1);
      line2->setText(x_value_);
    }
    if (str == "±" && !x_value_.isEmpty()) {
      Negate(x_value_);
      line2->setText(x_value_);
    } else if (str == "cos" || str == "sin" || str == "tan" || str == "acos" ||
               str == "asin" || str == "atan" || str == "sqrt" || str == "ln" ||
               str == "log") {
      x_value_ += str;
      x_value_ += "(";
      line2->setText(x_value_);
    } else if (str != "." && str != "=" && str != "±" && str != "del" &&
               str != "AC" && str != "x") {
      x_value_ += str;
      line2->setText(x_value_);
    } else if (str == "=" && !x_value_.isEmpty()) {
      ReplaceX(data_, x_value_);
      x_value_.clear();
      line1->setText(data_ + "=");
      double result;
      int err = Calculate_(result, data_);
      if (!err) {
        data_ = QString::number(result);
        line2->setText(data_);
        status = STATUS_RESULT;
      } else {
        data_.clear();
        line1->clear();
        line2->setText(kErr[err]);
        status = STATUS_DEFAULT;
      }
    }
  }
}

void DefaultCalc::Negate(QString& data) {
  if (data.isEmpty()) {
    data_ = "0";
  }
  if (data[0] == '-') {
    data[0] = '+';
  } else if (data[0] == '+') {
    data[0] = '-';
  } else {
    data = "-" + data;
  }
}

void DefaultCalc::SlotGraphClicked() {
  if (data_.isEmpty()) {
    line2->setText("for graph build input data first");
  } else {
    Graph* graph_window = new Graph(this, data_);
    graph_window->setModal(true);

    graph_window->exec();


  }
}
DefaultCalc::~DefaultCalc() {
  delete ui;
}
