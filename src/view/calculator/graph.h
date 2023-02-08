#ifndef GRAPH_H
#define GRAPH_H

#include <QDialog>
#include <QVector>

namespace Ui {
class Graph;
}

class Graph : public QDialog {
  Q_OBJECT

 public:
  explicit Graph(QWidget *parent = nullptr, QString data = "");
  ~Graph();

 public:
  QString func_;
  int x_min_ = -10, y_min_ = -10;
  int x_max_ = 10, y_max_ = 10;
  QVector<double> x_data_, y_data_;
  void CalculatePoints();

 private:
  Ui::Graph *ui;
 public slots:
  void SlotReplotButtonClicked();
};

#endif  // GRAPH_H
