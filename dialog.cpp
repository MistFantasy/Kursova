#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Dialog), IsCancelled(false)
{
  ui->setupUi(this);
  CheckBoxList = {
                   ui->SaunaCheckBox,
                   ui->AkvaParkCheckBox,
                   ui->MassageCheckBox
                 };
  LineEditList = {
                   ui->SurnameEdit,
                   ui->NameEdit,
                   ui->MiddleNameEdit
                 };
}

Dialog::~Dialog()
{
  delete ui;
}

//Метод, який повертає об'єкт класу PoolPass
PoolPass Dialog::GetNewClient() const
{
  return NewClient;
}

//Метод, який вертає бул, чи була натиснута клавіша Cancel
bool Dialog::GetIsCancelled() const
{
  return IsCancelled;
}

//Метод, який встановлює бул, кожен раз при запуску діалового вікна
//----------------------------------------------------------------------
void Dialog::SetIsCancelled(bool Cancel)
{
  IsCancelled = Cancel;
}

//Метод, для перевірки вхідних даних з LineEdit-ів
bool Dialog::CheckInputInformation(QString & ErrorMessage)
{
  if(ui->SurnameEdit->text() == "")
    {
      ErrorMessage = "Введіть прізвище, будь ласка.";
      return false;
    }
  if(ui->NameEdit->text() == "")
    {
      ErrorMessage = "Введіть ім'я, будь ласка.";
      return false;
    }
  if(ui->MiddleNameEdit->text() == "")
    {
      ErrorMessage = "Введіть по-батькові, будь ласка.";
      return false;
    }
  return true;
}

//Метод, який встановлює значення за замовчуванням в об'єктах
void Dialog::ClearDialog()
{
  for(int i = 0; i < CheckBoxList.size(); i++)
    CheckBoxList.at(i)->setChecked(false);
  for(int i = 0; i < LineEditList.size(); i++)
    LineEditList.at(i)->clear();
  ui->CardPeriodSpinBox->setValue(1);
  ui->MonthsVisitsSpinBox->setValue(1);
  ui->SessionDurSpinBox->setValue(1);
  ui->FamilyCheckBox->setChecked(false);
}

//Метод, при натисканні на кнопку окей
//В ньому відбувається перевірка на правильний ввід даних
//В ньому створюється новий елемент класу PoolPass
void Dialog::on_OKButton_clicked()
{
  QStringList NameList;
  QString ErrorMessage;
  QStringList AddServ = {"Сауна", "Аквапарк", "Масаж"}, TempAddServ;
  for(int i = 0; i < LineEditList.size(); i++)
    NameList.append(LineEditList.at(i)->text());
  if(!CheckInputInformation(ErrorMessage))
    {
      QMessageBox::warning(this, "Неправильний ввід даних",
                           ErrorMessage);
      return;
    }
  for(int i = 0; i < CheckBoxList.size(); i++)
    {
      if(CheckBoxList.at(i)->isChecked())
        TempAddServ.append(AddServ.at(i));
    }
  NewClient.SetAdditionaServices(TempAddServ);
  NewClient.SetFamilyPlan(ui->FamilyCheckBox->isChecked());
  NewClient.SetMonthsVisits(ui->MonthsVisitsSpinBox->value());
  NewClient.SetSessionDuration(ui->SessionDurSpinBox->value());
  NewClient.SetCardPeriod(ui->CardPeriodSpinBox->value());
  NewClient.SetName(NameList);
  ClearDialog();
  this->close();
}

//Кнопка Cancel, яка відміняє дії користувача
void Dialog::on_CancelButton_clicked()
{
  ClearDialog();
  IsCancelled = true;
  this->close();
}

