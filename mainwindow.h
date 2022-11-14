#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include "poolpass.h"
#include "dialog.h"
#include "writeanddelete.h"
#include "searchandoutput.h"
#include "help.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  //Функція для запису в таблицю
  void FillTable();

  //Функція, яка створює пустий елемент в таблиці
  void CreateNewElementInTable();
private slots:
  //Кнопка виклику діалогу додавання нового клієнта
  void on_AddNewElementButton_clicked();

  //Кнопка для додавання додаткових послуг
  void on_AddServicesButton_clicked();

  //Кнопка пошуку за ПІП або за к-стю відвідувань або за трив. перебування
  void on_SearchButton_clicked();

  //Кнопка для зчитування даних з файлу
  void on_EnterDataFromFileButton_clicked();

  //Кнопка для запису у файл
  void on_EnterDataInFileButton_clicked();

  //Виклик діалогового вікна, в якому вивести інформацію про абонемент
  void on_OutputButton_clicked();

  //Скасування виділення рядка таблиці при натисканні на клавішу миші
  void mousePressEvent(QMouseEvent *event);

  //Зчитування з файлу, при натисканні на кнопку в MenuBar
  void on_OpenFile_triggered();

  //Запис у файл, при натисканні на кнопку в MenuBar
  void on_SaveFile_triggered();

  //Вихід з програми, при натисканні на кнопку в MenuBar
  void on_Exit_triggered();

  //Додавання нового клієнта, при натисканні на кнопку в MenuBar
  void on_NewClientButton_triggered();

  //Вибір додаткових послуг, при натисканні на кнопку в MenuBar
  void on_AddServicesButtonMenuBarButton_triggered();

  //Пошук клієнта, при натисканні на кнопку в MenuBar
  void on_Search_triggered();

  //Вивід інформації про клієнта, при натисканні на кнопку в MenuBar
  void on_OutputInformationButton_triggered();

  void on_About_triggered();

private:
  Ui::MainWindow *ui;
  Dialog * DataClient;                    //Діалогове вікно для запису нового клієнта
  WriteAndDelete * WriteAndDeleteDialog;  //Діалогове вікно для вибору рядків для запису у файл,
                                          //для видалення рядків і для додавання додаткових послуг
  SearchAndOutput * SearchAndOutputDialog;//Діалогове вікно для пошуку за ПІП, к-стю відвідувань або за трив. перебування
                                          //Або для виводу інформації
  QVector<PoolPass> ListOfPoolPasses;     //Вектор об'єктів типу PoolPass
};
#endif // MAINWINDOW_H
