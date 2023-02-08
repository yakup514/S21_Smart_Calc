#include "graph.h"

#include <QDoubleValidator>
#include <QIntValidator>

#include "ui_graph.h"
#include "utils.h"
// #include "../../model/calc_core.h"

Graph::Graph(QWidget* parent, QString data)
    : QDialog(parent), ui(new Ui::Graph) {
  ui->setupUi(this);
  QDoubleValidator* double_vall =
      new QDoubleValidator(-10000000.0, 10000000.0, 2, this);
  QIntValidator* int_vall = new QIntValidator(1, 100, this);
  double_vall->setLocale(QLocale::English);
  int_vall->setLocale(QLocale::English);
  ui->x_max_line_edit->setValidator(double_vall);
  ui->x_min_line_edit->setValidator(double_vall);
  ui->y_max_line_edit->setValidator(double_vall);
  ui->y_min_line_edit->setValidator(double_vall);
  ui->line_points_num->setValidator(int_vall);
  ui->line_points_num->setText("10000");
  ui->line_func->setText(data);
  func_ = data;
  ui->widget->xAxis->setRange(x_min_, x_max_);
  ui->widget->yAxis->setRange(y_min_, y_max_);
  CalculatePoints();
  ui->widget->addGraph();
  ui->widget->graph(0)->addData(x_data_, y_data_);
  ui->widget->replot();
  connect(ui->replot_btn, SIGNAL(clicked()), this,
          SLOT(SlotReplotButtonClicked()));
}

Graph::~Graph() { delete ui; }

void Graph::CalculatePoints() {
  int point_num = ui->line_points_num->text().toInt();
  if (point_num != 0) {
    double step = (x_max_ - x_min_) / (double)point_num;
    double start_x = x_min_, end_x = x_max_;
    while (start_x <= end_x) {
      x_data_.push_back(start_x);
      QString temp = func_;
      ReplaceX(temp, QString::number(start_x));
      double qurr_y;
      int err = Calculate_(qurr_y, temp);
      // qDebug() << temp << "  " << start_x << "   " << qurr_y;
      if (!err && qurr_y >= y_min_ && qurr_y <= y_max_)
        y_data_.push_back(qurr_y);
      else
        y_data_.push_back(NAN);
      start_x += step;
    }
  }
}

void Graph::SlotReplotButtonClicked() {
  bool min_x, min_y, max_x, max_y;
  double temp_min_x = ui->x_min_line_edit->text().toDouble(&min_x);
  double temp_min_y = ui->y_min_line_edit->text().toDouble(&min_y);
  double temp_max_x = ui->x_max_line_edit->text().toDouble(&max_x);
  double temp_max_y = ui->y_max_line_edit->text().toDouble(&max_y);
  if (min_x && min_y && max_x && max_y && temp_max_x > temp_min_x &&
      temp_max_y > temp_min_y) {
    x_min_ = temp_min_x;
    x_max_ = temp_max_x;
    y_min_ = temp_min_y;
    y_max_ = temp_max_y;
    x_data_.clear();
    y_data_.clear();
    func_ = ui->line_func->text();
    CalculatePoints();
    ui->widget->xAxis->setRange(x_min_, x_max_);
    ui->widget->yAxis->setRange(y_min_, y_max_);

    ui->widget->clearGraphs();
    ui->widget->addGraph();
    ui->widget->graph(0)->addData(x_data_, y_data_);
    ui->widget->replot();
  }
}
