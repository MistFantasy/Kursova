#include "writeanddelete.h"
#include "ui_writeanddelete.h"

WriteAndDelete::WriteAndDelete(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::WriteAndDelete), IsCancelled(false), NumberOfRowInTable(0)
{
  ui->setupUi(this);
  ui->NumberLineEdit->setEnabled(false);
  CheckBoxList = {
                   ui->SaunaCheckBox,
                   ui->AkvaParkCheckBox,
                   ui->MassageCheckBox
                 };
}

WriteAndDelete::~WriteAndDelete()
{
  delete ui;
}

//Метод, який вертає бул, чи була натиснута клавіша Cancel
//----------------------------------------------------------------------
bool WriteAndDelete::GetIsCancelled() const
{
  return IsCancelled;
}

//Метод, який встановлює бул, кожен раз при запуску діалового вікна
//----------------------------------------------------------------------
void WriteAndDelete::SetIsCancelled(bool Cancel)
{
  IsCancelled = Cancel;
}

//Метод, який встановлює к-сть рядків, для перевірки на перевищення ліміту рядків
//----------------------------------------------------------------------
void WriteAndDelete::SetNumberOfRowInTable(int NumberOfRow)
{
  NumberOfRowInTable = NumberOfRow;
}

//Метод, для перевірки вхідних даних з SpinBox-a та LineEdit-a
//----------------------------------------------------------------------
bool WriteAndDelete::CheckInputInformation(QString & ErrorMessage, bool MethodOfReadValue)
{
  QStringList TestNumberList;
  if(!MethodOfReadValue)
    if(ui->NumberOfTheRowSpinBox->value() > NumberOfRowInTable)
      {
        ErrorMessage = "Введене число більше за к-сть рядків у таблиці, введіть будь ласка, ще раз.";
        return false;
      }
  if(MethodOfReadValue)
    {
    if(ui->NumberLineEdit->text() == "")
      {
        ErrorMessage = "Введіть числа, будь ласка.";
        return false;
      }
    for(int i = 0; i < ui->NumberLineEdit->text().size(); i++)
      {
        if(!ui->NumberLineEdit->text().at(i).isDigit()
           && ui->NumberLineEdit->text().at(i) != ','
           && ui->NumberLineEdit->text().at(i) != '-'
           && ui->NumberLineEdit->text().at(i) != ' ')
          {
            ErrorMessage = "Ви ввели числа неправильно, введіть, будь ласка, ще раз.";
            return false;
          }
      }
      TestNumberList = ui->NumberLineEdit->text().split(", ",
                                                    Qt::SkipEmptyParts,
                                                    Qt::CaseInsensitive);
    for(int i = 0; i < TestNumberList.size(); i++)
      {
        if(TestNumberList.at(i).contains('-'))
          {
            TestNumberList.append(TestNumberList.at(i).split('-'));
            TestNumberList.remove(i);
          }
        if(TestNumberList.at(i).toInt() > NumberOfRowInTable)
          {
            ErrorMessage = "Введене число більше, ніж кількість рядків у таблиці,";
            ErrorMessage = "введіть, будь ласка, ще раз.";
            return false;
          }
      }
    }
  return true;
}

//Метод, який повертає індекси вибраних рядків
//----------------------------------------------------------------------
QVector<int> WriteAndDelete::GetNumberOfRow() const
{
  return NumberOfRowToChoose;
}

//Метод, який встановлює значення за замовчуванням в об'єктах
//----------------------------------------------------------------------
void WriteAndDelete::ClearDialog()
{
  ui->MoreThanOneCheckBox->setChecked(false);
  ui->NumberLineEdit->setText("");
  ui->NumberOfTheRowSpinBox->setValue(1);
  ui->NumberOfTheRowSpinBox->setEnabled(true);
  ui->NumberLineEdit->setEnabled(false);
}

//Метод, який встановлює видимість від чекбоксів, залежно яку функцію
//хочемо викликати, або запис у файл, або додавання послуг
//----------------------------------------------------------------------
void WriteAndDelete::SetCheckBoxesVisible(bool IsVisible)
{
  ui->SaunaCheckBox->setVisible(IsVisible);
  ui->AkvaParkCheckBox->setVisible(IsVisible);
  ui->MassageCheckBox->setVisible(IsVisible);
}

//Метод, який повератає масив стрічок вибраних додаткових послуг
//----------------------------------------------------------------------
QStringList WriteAndDelete::GetAddServices()
{
  QStringList AddServ = {"Сауна", "Аквапарк", "Масаж"}, TempAddServ;
  for(int i = 0; i < CheckBoxList.size(); i++)
    {
      if(CheckBoxList.at(i)->isChecked())
        TempAddServ.append(AddServ.at(i));
    }
  return TempAddServ;
}

//Метод, при натисканні на кнопку окей
//В ньому відбувається перевірка на правильний ввід даних
//Та в ньому зчитуються індекси вибраних рядків
//----------------------------------------------------------------------
void WriteAndDelete::on_OKButton_clicked()
{
  NumberOfRowToChoose.clear();
  QString ErrorMessage;
  if(!CheckInputInformation(ErrorMessage, ui->MoreThanOneCheckBox->isChecked()))
    {
      QMessageBox::warning(this, "Неправильний ввід даних",
                           ErrorMessage);
      return;
    }
  if(ui->MoreThanOneCheckBox->isChecked())
    {
      QStringList TempNumberList = ui->NumberLineEdit->text().split(
            ", ", Qt::SkipEmptyParts, Qt::CaseInsensitive);
      for(int i = 0; i < TempNumberList.size(); i++)
        {
          if(TempNumberList.at(i).contains('-'))
            {
              for (int j = TempNumberList.at(i).at(0).digitValue();
                   j <= TempNumberList.at(i).at(2).digitValue(); j++)
                {
                  NumberOfRowToChoose.append(j - 1);
                }
            }
          else
            NumberOfRowToChoose.append(TempNumberList.at(i).toInt() - 1);
        }
    }
  if(!ui->MoreThanOneCheckBox->isChecked())
    {
      NumberOfRowToChoose.append(ui->NumberOfTheRowSpinBox->value() - 1);
    }
  ClearDialog();
  this->close();
}

//Вибір з чого вводити дані, або з SpinBox-a при цьому блокується
//LineEdit на формі, або навпаки
//----------------------------------------------------------------------
void WriteAndDelete::on_MoreThanOneCheckBox_clicked()
{
  if(ui->MoreThanOneCheckBox->isChecked())
    {
      ui->NumberOfTheRowSpinBox->setEnabled(false);
      ui->NumberLineEdit->setEnabled(true);
    }
  if(!ui->MoreThanOneCheckBox->isChecked())
    {
      ui->NumberLineEdit->setEnabled(false);
      ui->NumberOfTheRowSpinBox->setEnabled(true);
    }
}


//Кнопка Cancel, яка відміняє дії користувача
//----------------------------------------------------------------------
void WriteAndDelete::on_CancelButton_clicked()
{
  NumberOfRowToChoose.clear();
  ClearDialog();
  for(int i = 0; i < CheckBoxList.size(); i++)
    CheckBoxList.at(i)->setChecked(false);
  IsCancelled = true;
  this->close();
}

