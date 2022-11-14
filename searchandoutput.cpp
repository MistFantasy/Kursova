#include "searchandoutput.h"
#include "ui_searchandoutput.h"

SearchAndOutput::SearchAndOutput(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SearchAndOutput), SessionDuration(0),
  MonthsVisits(0), IsCancelled(false)
{
  ui->setupUi(this);
}

SearchAndOutput::~SearchAndOutput()
{
  delete ui;
}

//Метод, який встановлює значення за замовчуванням в об'єктах діалогового вікна
//----------------------------------------------------------------------
void SearchAndOutput::ClearDialog()
{
  ui->MonthsVisitsCheckBox->setChecked(false);
  ui->MonthsVisitsSpinBox->setValue(1);
  ui->MonthsVisitsSpinBox->setEnabled(false);
  ui->NameLineEdit->setText("");
  ui->NameLineEdit->setEnabled(false);
  ui->OutputTextBrowser->setText("");
  ui->PIPCheckBox->setChecked(false);
  ui->SessionDurCheckBox->setChecked(false);
  ui->SessionDurSpinBox->setValue(1);
  ui->SessionDurSpinBox->setEnabled(false);
  Name.clear();
  SessionDuration = 0;
  MonthsVisits = 0;
  ListOfPoolPasses.clear();
}

//Метод, який встановлює елементи відповідної функції
//----------------------------------------------------------------------
void SearchAndOutput::SwitchVisible(QString WhatMethod)
{
  if(WhatMethod == "Search")
    {
      ui->OutputComboBox->setVisible(false);
      ui->OutputTextBrowser->setVisible(false);
      ui->OutputButton->setVisible(false);
      ui->MonthsVisitsCheckBox->setVisible(true);
      ui->MonthsVisitsSpinBox->setVisible(true);
      ui->NameLineEdit->setVisible(true);
      ui->PIPCheckBox->setVisible(true);
      ui->SessionDurCheckBox->setVisible(true);
      ui->SessionDurSpinBox->setVisible(true);
    }
  if(WhatMethod == "Output")
    {
      ui->MonthsVisitsCheckBox->setVisible(false);
      ui->MonthsVisitsSpinBox->setVisible(false);
      ui->NameLineEdit->setVisible(false);
      ui->PIPCheckBox->setVisible(false);
      ui->SessionDurCheckBox->setVisible(false);
      ui->SessionDurSpinBox->setVisible(false);
      ui->OutputComboBox->setVisible(true);
      ui->OutputTextBrowser->setVisible(true);
      ui->OutputButton->setVisible(true);
    }
}

//Метод, який вертає бул, чи була натиснута клавіша Cancel
//----------------------------------------------------------------------
bool SearchAndOutput::GetIsCancelled() const
{
  return IsCancelled;
}

//Метод, який встановлює бул, кожен раз при запуску діалового вікна
//----------------------------------------------------------------------
void SearchAndOutput::SetIsCancelled(bool Cancel)
{
  IsCancelled = Cancel;
}

//Метод, який встановлює список об'єктів класу PoolPass
//----------------------------------------------------------------------
void SearchAndOutput::SetListOfPoolPasses(QVector<PoolPass> ListOfPasses)
{
  ListOfPoolPasses = ListOfPasses;
}

//Гетер, який повертає вказівник на ім'я
//----------------------------------------------------------------------
const QString *SearchAndOutput::GetName() const
{
  if(Name == "")
    return nullptr;
  else
    return &Name;
}

//Гетер, який повертає вказівник на тривалість перебування
//----------------------------------------------------------------------
const int *SearchAndOutput::GetSessionDuration() const
{
  if(SessionDuration == 0)
    return nullptr;
  else
    return &SessionDuration;
}

//Гетер, який повертає вказівник на к-сть відвідувань в місяць
//----------------------------------------------------------------------
const int *SearchAndOutput::GetMonthsVisits() const
{
  if(MonthsVisits == 0)
    return nullptr;
  else
    return &MonthsVisits;
}

//Додаткова функція виводу об'єкту у TextBrowser, який знаходиться в діалоговому вікні
//----------------------------------------------------------------------
void SearchAndOutput::OutputInformation(int Index)
{
  ui->OutputTextBrowser->insertPlainText(
        "Ім'я: " + ListOfPoolPasses.at(Index).GetName() + "\n");
  ui->OutputTextBrowser->insertPlainText(
        "Період дії картки: " + QString::number(
          ListOfPoolPasses.at(Index).GetCardPeriod()) + "\n");
  ui->OutputTextBrowser->insertPlainText(
        "Тривалість перебування: " + QString::number(
          ListOfPoolPasses.at(Index).GetSessionDuration()) + "\n");
  ui->OutputTextBrowser->insertPlainText(
        "К-сть відвідувань в місяць: " + QString::number(
          ListOfPoolPasses.at(Index).GetMonthsVisits()) + "\n");
  if(ListOfPoolPasses.at(Index).GetFamilyPlan())
    ui->OutputTextBrowser->insertPlainText("Сімейний тариф: присутній\n");
  else
    ui->OutputTextBrowser->insertPlainText("Сімейний тариф: відсутній\n");
  ui->OutputTextBrowser->insertPlainText("Додаткові послуги: " +
                                         ListOfPoolPasses.at(Index).GetAdditionalServices() + "\n");
}

//Функція, яка виводить інформацію згідно з завданням
//----------------------------------------------------------------------
void SearchAndOutput::TextBrowserOutput()
{
  int IndexOfPoolPasses = -1;
  //Вивід абонемента, в якого закінчується період дії картки
  //та містить додаткову послугу "Аквапарк" одночасно
  if(ui->OutputComboBox->currentIndex() == 0)
    {
      for(int i = 0; i < ListOfPoolPasses.size(); i++)
        {
          if(ListOfPoolPasses.at(i).GetCardPeriod() < 30 &&
             ListOfPoolPasses.at(i).GetAdditionalServices().contains("Аквапарк"))
            {
              OutputInformation(i);
              IndexOfPoolPasses = i;
            }
        }
    }
  //Вивід абонемента, в якого найбільше додаткових послуг
  //та найменша тривалість перебування
  if(ui->OutputComboBox->currentIndex() == 1)
    {
      int MaxAddServices = 0,
          MinSessionDur = 0;
      const QStringList TempStringList = {"Сауна", "Аквапарк", "Масаж"};
      for (int i = 0; i < ListOfPoolPasses.size(); i++)
        {
          int TempMaxAddServices = 0;
          for(int j = 0; j < TempStringList.size(); j++)
            {
              if(ListOfPoolPasses.at(i).GetAdditionalServices().contains(TempStringList.at(j)))
                TempMaxAddServices++;
            }
          if(TempMaxAddServices >= MaxAddServices)
            {
              MaxAddServices = TempMaxAddServices;
              if(ListOfPoolPasses.at(i).GetSessionDuration() <=
                 ListOfPoolPasses.at(MinSessionDur).GetSessionDuration())
                {
                  MinSessionDur = i;
                  IndexOfPoolPasses = i;
                }
            }
        }
      if(IndexOfPoolPasses != -1)
        OutputInformation(IndexOfPoolPasses);
    }
  //Вивід абонемента, що має найбільшу к-сть відвідувань в місяць
  //та є сімейним одночасно
  if(ui->OutputComboBox->currentIndex() == 2)
    {
      int MaxMonthsVisits = 0;
      for(int i = 0; i < ListOfPoolPasses.size(); i++)
        {
          if(ListOfPoolPasses.at(i).GetMonthsVisits() >=
             ListOfPoolPasses.at(MaxMonthsVisits).GetMonthsVisits()
             && ListOfPoolPasses.at(i).GetFamilyPlan())
            {
              MaxMonthsVisits = i;
              IndexOfPoolPasses = i;
            }
        }
      if(IndexOfPoolPasses != -1)
        OutputInformation(IndexOfPoolPasses);
    }
  if(IndexOfPoolPasses == -1)
    {
      QMessageBox::warning(this, "Абонемент в басейн",
                           "Перевірте, чи правильно введені дані.");
    }
}

//Метод, для перевірки вхідних даних з LineEdit-ів
//----------------------------------------------------------------------
bool SearchAndOutput::CheckInputInformation(QString & ErrorMessage)
{
  if(ui->NameLineEdit->text() == "" && ui->PIPCheckBox->isChecked())
    {
      ErrorMessage = "Введіть ПІП, будь ласка.";
      return false;
    }
  return true;
}

//Метод, який викликається при натисканні на кнопку OK
//---------------------------------------------------------------------
void SearchAndOutput::on_OKButton_clicked()
{
  QString ErrorMessage;
  if(!CheckInputInformation(ErrorMessage))
    {
      QMessageBox::warning(this, "Неправильний ввід даних",
                           ErrorMessage);
      return;
    }
  if(ui->PIPCheckBox->isChecked())
    Name = ui->NameLineEdit->text();
  if(ui->SessionDurCheckBox->isChecked())
    SessionDuration = ui->SessionDurSpinBox->value();
  if(ui->MonthsVisitsCheckBox->isChecked())
    MonthsVisits = ui->MonthsVisitsSpinBox->value();
  this->close();
}

//Метод, який викликається при натисканні на кнопку Cancel
//----------------------------------------------------------------------
void SearchAndOutput::on_CancelButton_clicked()
{
  ClearDialog();
  IsCancelled = true;
  this->close();
}

//Метод, який викликається при натисканні на кнопку "Вивести дані"
//----------------------------------------------------------------------
void SearchAndOutput::on_OutputButton_clicked()
{
  TextBrowserOutput();
}

//Метод, який активовує спінбокс, який відповідає за к-сть відвідувань
//----------------------------------------------------------------------
void SearchAndOutput::on_MonthsVisitsCheckBox_clicked()
{
  if(ui->MonthsVisitsCheckBox->isChecked())
    ui->MonthsVisitsSpinBox->setEnabled(true);
  else
    ui->MonthsVisitsSpinBox->setEnabled(false);
}

//Метод, який активовує спінбокс, який відповідає за тривалість перебування
//----------------------------------------------------------------------
void SearchAndOutput::on_SessionDurCheckBox_clicked()
{
  if(ui->SessionDurCheckBox->isChecked())
    ui->SessionDurSpinBox->setEnabled(true);
  else
    ui->SessionDurSpinBox->setEnabled(false);
}

//Метод, який активовує спінбокс, який відповідає за ПІП
//----------------------------------------------------------------------
void SearchAndOutput::on_PIPCheckBox_clicked()
{
  if(ui->PIPCheckBox->isChecked())
    ui->NameLineEdit->setEnabled(true);
  else
    ui->NameLineEdit->setEnabled(false);
}

