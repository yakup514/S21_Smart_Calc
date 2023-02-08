// #include "mainwindow.h"
// #include "calc.h"
#include <QApplication>
#include <QFile>
#include <QVBoxLayout>
#include <QDir>


#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QFile style_file;
  QString str = QDir::currentPath();
  style_file.setFileName(
      str + "/../../../../C7_SmartCalc_v1.0-0/src/view/calculator/style.qss");
  if (style_file.open(QFile::ReadOnly)) {
    QString style(style_file.readAll());
    a.setStyleSheet(style);
    style_file.close();
  } else {
    qDebug() << "not opened style";
  }
  MainWindow mw;
  mw.show();

  return a.exec();
}
