#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "poolpass.h"
#include <QLineEdit>
namespace Ui {
  class Dialog;
}

class Dialog : public QDialog
{
  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = nullptr);
  ~Dialog();

  //Метод, який повертає об'єкт класу PoolPass
  PoolPass GetNewClient() const;

  //Метод, який вертає бул, чи була натиснута клавіша Cancel
  bool GetIsCancelled() const;

  //Метод, для перевірки вхідних даних з LineEdit-ів
  bool CheckInputInformation(QString &ErrorMessage);

  //Метод, який встановлює значення за замовчуванням в об'єктах
  void ClearDialog();

  //Метод, який встановлює бул, кожен раз при запуску діалового вікна
  void SetIsCancelled(bool Cancel);
private slots:

  //Метод, при натисканні на кнопку окей
  //В ньому відбувається перевірка на правильний ввід даних
  //В ньому створюється новий елемент класу PoolPass
  void on_OKButton_clicked();

  //Кнопка Cancel, яка відміняє дії користувача
  void on_CancelButton_clicked();

private:
  PoolPass NewClient; //Тимчасовий об'єкт класу PoolPass для створення нового клієнта
  QVector<QCheckBox *> CheckBoxList; //Список Чекбоксів
  QVector<QLineEdit *> LineEditList; //Список Лайнедітів
  bool IsCancelled;                  //Булева змінна, яка вказує чи була натиснута кнопка Cancel
  Ui::Dialog *ui;
};

#endif // DIALOG_H
