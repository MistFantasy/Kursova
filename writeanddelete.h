#ifndef WRITEANDDELETE_H
#define WRITEANDDELETE_H

#include <QDialog>
#include <QMessageBox>
namespace Ui {
  class WriteAndDelete;
}

class WriteAndDelete : public QDialog
{
  Q_OBJECT

public:
  explicit WriteAndDelete(QWidget *parent = nullptr);
  ~WriteAndDelete();

  //Метод, який вертає бул, чи була натиснута клавіша Cancel
  bool GetIsCancelled() const;

  //Метод, який повертає масив стрічок вибраних додаткових послуг
  QStringList GetAddServices();

  //Метод, який встановлює к-сть рядків, для перевірки на перевищення ліміту рядків
  void SetNumberOfRowInTable(int NumberOfRow);

  //Метод, який повертає індекси вибраних рядків
  QVector<int> GetNumberOfRow() const;

  //Метод, для перевірки вхідних даних з SpinBox-a та LineEdit-a
  bool CheckInputInformation(QString &ErrorMessage, bool MethodOfReadValue);

  //Метод, який встановлює значення за замовчуванням в об'єктах
  void ClearDialog();

  //Метод, який встановлює видимість від чекбоксів, залежно яку функцію
  //хочемо викликати, або запис у файл, або додавання послуг
  void SetCheckBoxesVisible(bool IsVisible);

  //Метод, який встановлює бул, кожен раз при запуску діалового вікна
  void SetIsCancelled(bool Cancel);
private slots:

  //Метод, при натисканні на кнопку окей
  //В ньому відбувається перевірка на правильний ввід даних
  //Та в ньому зчитуються індекси вибраних рядків
  void on_OKButton_clicked();

  //Вибір з чого вводити дані, або з SpinBox-a при цьому блокується
  //LineEdit на формі, або навпаки
  void on_MoreThanOneCheckBox_clicked();

  //Кнопка Cancel, яка відміняє дії користувача
  void on_CancelButton_clicked();

private:
  Ui::WriteAndDelete *ui;
  bool IsCancelled;     //Змінна, яка визначає чи була натиснута клавіша Cancel
  QVector<QCheckBox *> CheckBoxList;  //Список Чекбоксів, які містять додаткові послуги
  QVector<int> NumberOfRowToChoose;   //Список індексів вибраних рядків
  int NumberOfRowInTable;             //К-сть рядків в таблиці
};

#endif // WRITEANDDELETE_H
