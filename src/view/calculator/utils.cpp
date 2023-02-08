#include "utils.h"

#include "../../model/calc_core.h"

void ReplaceX(QString &data, QString x_value) {
  qsizetype start_pos = 0;

  x_value = "(" + x_value + ")";

  while ((start_pos = data.indexOf("x", start_pos)) != -1) {
    data.replace(start_pos, 1, x_value);
  }
}

int Calculate_(double &result, QString data) {
  int err = 0;
  std::string temp = data.toStdString();

  char *buff = new char[temp.size() + 1];

  for (int i = 0; temp[i] != '\0'; ++i) {
    buff[i] = temp[i];
  }
  buff[temp.size()] = '\0';
  err = Calculate(buff, &result);
  delete[] buff;
  return err;
}
