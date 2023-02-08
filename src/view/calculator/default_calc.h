#ifndef DEFAULT_CALC_H
#define DEFAULT_CALC_H

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#define STATUS_DEFAULT 0
#define STATUS_RESULT 1
#define STATUS_XVAL 2

namespace Ui {
class DefaultCalc;
}

class DefaultCalc : public QWidget {
  Q_OBJECT

 public:
  explicit DefaultCalc(QWidget *parent = nullptr);
  ~DefaultCalc();
  QPushButton *CreateButton(const QString &str);
  bool is_show;
  // Graph graph_window;

  QString data_;

 private:
  int status = STATUS_DEFAULT;
  bool ent_x_ = false;
  bool line2_has_result_ = false;
  QString x_value_;
  QLineEdit *line1;
  QLineEdit *line2;
  void Negate(QString &data);

 public slots:
  void SlotButtonClicked();
  void SlotGraphClicked();

 private:
  Ui::DefaultCalc *ui;
};

const static QString kErr[7] = {
    "",           "OPERATION_ERROR", "FUNCTION_ERROR", "PARENTHESIS_ERROR",
    "CALC_ERROR", "PARSE_ERROR",     "LENGTH_ERROR"};

#endif  // DEFAULT_CALC_H
