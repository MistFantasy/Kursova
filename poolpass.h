#ifndef POOLPASS_H
#define POOLPASS_H
#include <QStringList>
#include <QTextStream>
#include <QMessageBox>
#include <iostream>
#define FILESIZE 256
class PoolPass
{
public:
  //Конструктор за замовчуванням
  PoolPass();

  //Конструктор з параметрами
  PoolPass(bool Family, int MonthsVisits, int SessionDuration,
           int CardPeriod, QStringList Names);

  //Конструктор копіювання
  PoolPass(const PoolPass &rOther);

  //Гетер, який повертає додаткові послуги
  QString GetAdditionalServices() const;

  //Гетер, який повертає сімейний тариф
  bool GetFamilyPlan() const;

  //Гетер, який повертає к-сть відвідувань в місяць
  int GetMonthsVisits() const;

  //Гетер, який повертає тривалість перебування в день
  int GetSessionDuration() const;

  //Гетер, який повертає період дії картки
  int GetCardPeriod() const;

  //Гетер, який повертає прізвище, ім'я, по-батькові
  QString GetName() const;

  //Сетер, який встановлює додаткові послуги
  void SetAdditionaServices(QStringList AdditionalServices);

  //Сетер, який встановлює сімейний тариф
  void SetFamilyPlan(bool FamilyPlan);

  //Сетер, який встановлює к-сть відвідувань за місяць
  void SetMonthsVisits(int MonthsVisits);

  //Сетер, який встановлює тривалість перебування за день
  void SetSessionDuration(int SessionDuration);

  //Сетер, який встановлює період дії картки
  void SetCardPeriod(int CardPeriod);

  //Сетер, який встановлює прізвище, ім'я, по-батькові
  void SetName(QStringList Names);

  //Перевантажений оператор виводу у файл
  friend QTextStream & operator <<(QTextStream & Out, const PoolPass &rOther);

  //Перевантажений оператор вводу з файлу
  friend QTextStream & operator >>(QTextStream & In, PoolPass &rOther);

  //Метод для очищення стрічок
  void Clear();

  //Метод, який порівнює шукане значення з полями об'єкту
  //Якщо хоча б одне значення співпадає вертаю true
  bool SearchToCompare(const QString * SearchedString,
                       const int * SearchedSessionDur,
                       const int * SearchedMonthsVisits);
private:
  QStringList m_AdditionalServices; // Масив стрічок, який містить додаткові послуги
  bool m_Family;                    // Сімейний тариф
  int m_MonthsVisits;               // К-сть відвідувань в місяць
  int m_SessionDuration;            // Тривалість перебування в день, в годинах
  int m_CardPeriod;                 // Період дії картки, в днях
  QStringList m_Name;               // Масив стрічок, який містить прізвище, ім'я, по-батькові
};
#endif // POOLPASS_H
