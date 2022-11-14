#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  DataClient = new Dialog(this);
  WriteAndDeleteDialog = new WriteAndDelete(this);
  SearchAndOutputDialog = new SearchAndOutput(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

//Створення нового елемента таблиці, при натисканні на кнопку
//та виклик діалогу, в якому можна ввести дані нового клієнта
//----------------------------------------------------------------------
void MainWindow::on_AddNewElementButton_clicked()
{
  DataClient->setModal(true);
  DataClient->exec();
  if(DataClient->GetIsCancelled())
    {
      DataClient->SetIsCancelled(false);
      return;
    }
  ListOfPoolPasses.append(DataClient->GetNewClient());
  FillTable();
  ui->TablePoolPass->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

//Виклик діалогового вікна, в якому можна додати послуги
//----------------------------------------------------------------------
void MainWindow::on_AddServicesButton_clicked()
{
  WriteAndDeleteDialog->SetNumberOfRowInTable(ui->TablePoolPass->rowCount());
  WriteAndDeleteDialog->setWindowTitle("Додавання послуг");
  WriteAndDeleteDialog->SetCheckBoxesVisible(true);
  WriteAndDeleteDialog->setModal(true);
  WriteAndDeleteDialog->exec();
  if(WriteAndDeleteDialog->GetIsCancelled())
    {
      WriteAndDeleteDialog->SetIsCancelled(false);
      return;
    }
  for(int i = 0; i < WriteAndDeleteDialog->GetNumberOfRow().size(); i++)
    {
      QStringList TempStringList = WriteAndDeleteDialog->GetAddServices();
      int Index = WriteAndDeleteDialog->GetNumberOfRow().at(i);
      ListOfPoolPasses[Index].SetAdditionaServices(TempStringList);
      ui->TablePoolPass->item(Index, 5)->
          setText(ListOfPoolPasses[Index].GetAdditionalServices());
    }
}

//Виклик діалогового вікна, в якому можна знайти клієнта
//----------------------------------------------------------------------
void MainWindow::on_SearchButton_clicked()
{
  SearchAndOutputDialog->SwitchVisible("Search");
  SearchAndOutputDialog->setWindowTitle("Шукати");
  SearchAndOutputDialog->setModal(true);
  SearchAndOutputDialog->exec();
  if(SearchAndOutputDialog->GetIsCancelled())
    {
      SearchAndOutputDialog->SetIsCancelled(false);
      return;
    }
  const QString * Name = SearchAndOutputDialog->GetName();
  const int * SessionDur = SearchAndOutputDialog->GetSessionDuration();
  const int * MonthsVisits = SearchAndOutputDialog->GetMonthsVisits();
  bool IsFound = false;
  for(int i = 0; i < ListOfPoolPasses.size(); i++)
    {
      if(ListOfPoolPasses[i].SearchToCompare(Name, SessionDur, MonthsVisits))
        {
          for (int j = 0; j < ui->TablePoolPass->columnCount(); j++)
            {
              ui->TablePoolPass->item(i, j)->setSelected(true);
            }
          IsFound = true;
        }
    }
  if(!IsFound)
    {
      QMessageBox::information(this,
                               "Абонемент в басейн",
                               "Клієнта не знайдено, перевірте введені дані.");
    }
  SearchAndOutputDialog->ClearDialog();
}

//Виклик діалогового вікна, в якому вивести інформацію про абонемент
//----------------------------------------------------------------------
void MainWindow::on_OutputButton_clicked()
{
  SearchAndOutputDialog->SwitchVisible("Output");
  SearchAndOutputDialog->setWindowTitle("Вивести інформацію");
  SearchAndOutputDialog->SetListOfPoolPasses(ListOfPoolPasses);
  SearchAndOutputDialog->setModal(true);
  SearchAndOutputDialog->exec();
  if(SearchAndOutputDialog->GetIsCancelled())
    {
      SearchAndOutputDialog->SetIsCancelled(false);
      return;
    }
  SearchAndOutputDialog->ClearDialog();
}

//Скасування виділення рядка таблиці при натисканні на клавішу миші
//----------------------------------------------------------------------
void MainWindow::mousePressEvent(QMouseEvent *event)
{
  for(int i = 0; i < ui->TablePoolPass->rowCount(); i++)
    for(int j = 0; j < ui->TablePoolPass->columnCount(); j++)
      ui->TablePoolPass->item(i, j)->setSelected(false);
  Q_UNUSED(event);
}

//Запис у файл, при натисканні на кнопку
//----------------------------------------------------------------------
void MainWindow::on_EnterDataInFileButton_clicked()
{
  QString FileName;
  WriteAndDeleteDialog->SetNumberOfRowInTable(ui->TablePoolPass->rowCount());
  WriteAndDeleteDialog->setWindowTitle("Зберегти файл");
  WriteAndDeleteDialog->SetCheckBoxesVisible(false);
  WriteAndDeleteDialog->setModal(true);
  WriteAndDeleteDialog->exec();
  if(WriteAndDeleteDialog->GetIsCancelled())
    {
      WriteAndDeleteDialog->SetIsCancelled(false);
      return;
    }
  FileName = QFileDialog::getOpenFileName(this, "Зберегти файл",
  "E:\\QT\\Projects\\Kursova_PZ_22_Riznyk\\build-Kursova-Desktop_Qt_6_1_2_MinGW_64_bit-Debug",
                                          "Data Files (*.dat);; All Files (*.*)", nullptr, QFileDialog::DontUseNativeDialog);
  try
  {
    if(!FileName.contains(".dat"))
      throw "Can't read File";
  }
  catch (const char *)
  {
    QMessageBox::critical(this, "Абонемент в басейн", "Не вдалося відкрити файл. "
"Перевірте, чи ви вибрали правильний файл.");
    return;
  }
  QFile OutputFile(FileName);
  OutputFile.open(QIODevice::WriteOnly | QIODevice::Append
                     | QIODevice::Text);
  QTextStream Out;
  Out.setDevice(&OutputFile);
  for(int i = 0; i < WriteAndDeleteDialog->GetNumberOfRow().size(); i++)
    Out << ListOfPoolPasses.at(WriteAndDeleteDialog->GetNumberOfRow().at(i));
  OutputFile.close();
}

//Зчитування з файлу, при натисканні на кнопку
//----------------------------------------------------------------------
void MainWindow::on_EnterDataFromFileButton_clicked()
{
  QString FileName;
  FileName = QFileDialog::getOpenFileName(this, "Відкрити файл",
  "E:\\QT\\Projects\\Kursova_PZ_22_Riznyk\\build-Kursova-Desktop_Qt_6_1_2_MinGW_64_bit-Debug",
                                          "Data Files (*.dat);; All Files (*.*)", nullptr, QFileDialog::DontUseNativeDialog);
  try
  {
    if(!FileName.contains(".dat"))
      throw "Can't read File";
  }
  catch (const char *)
  {
    QMessageBox::critical(this, "Абонемент в басейн", "Не вдалося відкрити файл. "
"Перевірте, чи ви вибрали правильний файл.");
    return;
  }
    QFile InputFile(FileName);
    InputFile.open(QIODevice::ReadOnly);
    QTextStream In;
    In.setDevice(&InputFile);
    PoolPass TempPoolPass;
    while(!In.atEnd())
      {
        In >> TempPoolPass;
        ListOfPoolPasses.push_back(TempPoolPass);
        TempPoolPass.Clear();
      }
    InputFile.close();
    FillTable();
    ui->TablePoolPass->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

//Функція, яка створює пустий елемент в таблиці
void MainWindow::CreateNewElementInTable()
{
  ui->TablePoolPass->setRowCount(ui->TablePoolPass->rowCount() + 1);
  for(int i = 0; i < ui->TablePoolPass->columnCount(); i++)
    ui->TablePoolPass->setItem(ui->TablePoolPass->rowCount() - 1, i,
                               new QTableWidgetItem(""));
  ui->TablePoolPass->item(ui->TablePoolPass->rowCount() - 1, 4)->setCheckState(Qt::Unchecked);
}

//Функція для запису в таблицю
//Якщо таблиця пуста, то з нуля починати цикл
//Якщо ні, то з номера останнього елемента ListOfPoolPasses
//----------------------------------------------------------------------
void MainWindow::FillTable()
{
  int CountOfElements = ui->TablePoolPass->rowCount();
  for(int i = CountOfElements; i < ListOfPoolPasses.size(); i++)
    {
      CreateNewElementInTable();
      ui->TablePoolPass->item(i, 5)->setText
          (ListOfPoolPasses.at(i).GetAdditionalServices());
      if(ListOfPoolPasses.at(i).GetFamilyPlan())
        ui->TablePoolPass->item(i, 4)->setCheckState(Qt::Checked);
      if(!ListOfPoolPasses.at(i).GetFamilyPlan())
        ui->TablePoolPass->item(i, 4)->setCheckState(Qt::Unchecked);
      ui->TablePoolPass->item(i, 3)->setText(
          QString::number(ListOfPoolPasses.at(i).GetMonthsVisits()));
      ui->TablePoolPass->item(i, 2)->setText(
          QString::number(ListOfPoolPasses.at(i).GetSessionDuration()));
      ui->TablePoolPass->item(i, 1)->setText(
          QString::number(ListOfPoolPasses.at(i).GetCardPeriod()));
      ui->TablePoolPass->item(i, 0)->setText(
            ListOfPoolPasses.at(i).GetName());
      for(int j = 0; j < ui->TablePoolPass->columnCount(); j++)
        ui->TablePoolPass->item(i, j)->setTextAlignment(Qt::AlignCenter);
    }
}

//Зчитування з файлу, при натисканні на кнопку в MenuBar
//----------------------------------------------------------------------
void MainWindow::on_OpenFile_triggered()
{
  on_EnterDataFromFileButton_clicked();
}

//Запис у файл, при натисканні на кнопку в MenuBar
//----------------------------------------------------------------------
void MainWindow::on_SaveFile_triggered()
{
  on_EnterDataInFileButton_clicked();
}

//Вихід з програми, при натисканні на кнопку в MenuBar
//----------------------------------------------------------------------
void MainWindow::on_Exit_triggered()
{
  this->close();
}

//Додавання нового клієнта, при натисканні на кнопку в MenuBar
//----------------------------------------------------------------------
void MainWindow::on_NewClientButton_triggered()
{
  on_AddNewElementButton_clicked();
}

//Вибір додаткових послуг, при натисканні на кнопку в MenuBar
//----------------------------------------------------------------------
void MainWindow::on_AddServicesButtonMenuBarButton_triggered()
{
  on_AddServicesButton_clicked();
}

//Пошук клієнта, при натисканні на кнопку в MenuBar
//----------------------------------------------------------------------
void MainWindow::on_Search_triggered()
{
  on_SearchButton_clicked();
}

//Вивід інформації про клієнта, при натисканні на кнопку в MenuBar
//----------------------------------------------------------------------
void MainWindow::on_OutputInformationButton_triggered()
{
  on_OutputButton_clicked();
}

void MainWindow::on_About_triggered()
{
  Help * HelpDialog = new Help(this);
  HelpDialog->show();
}

