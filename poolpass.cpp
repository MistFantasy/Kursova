#include "poolpass.h"

//Конструктор за замовчуванням
//----------------------------------------------------------------------
PoolPass::PoolPass()
 : m_Family(false), m_MonthsVisits(0), m_SessionDuration(0),
   m_CardPeriod(0)
{
}

//Конструктор з параметрами
//----------------------------------------------------------------------
PoolPass::PoolPass(bool Family, int MonthsVisits, int SessionDuration,
                   int CardPeriod, QStringList Names)
  : m_Family(Family), m_MonthsVisits(MonthsVisits),
    m_SessionDuration(SessionDuration), m_CardPeriod(CardPeriod), m_Name(Names)
{
}

//Конструктор копіювання
//----------------------------------------------------------------------
PoolPass::PoolPass(const PoolPass &rOther)
{
  this->m_AdditionalServices = rOther.m_AdditionalServices;
  this->m_CardPeriod = rOther.m_CardPeriod;
  this->m_Family = rOther.m_Family;
  this->m_MonthsVisits = rOther.m_MonthsVisits;
  this->m_Name = rOther.m_Name;
  this->m_SessionDuration = rOther.m_SessionDuration;
}

//Гетер, який повертає додаткові послуги
//----------------------------------------------------------------------
QString PoolPass::GetAdditionalServices() const
{
  QString Temp;
  for(int i = 0; i < m_AdditionalServices.size(); i++)
    Temp.append(m_AdditionalServices.at(i) + ", ");
  Temp.erase(Temp.end() - 2, Temp.end());
  return Temp;
}

//Гетер, який повертає сімейний тариф
//----------------------------------------------------------------------
bool PoolPass::GetFamilyPlan() const
{
  return m_Family;
}

//Гетер, який повертає к-сть відвідувань в місяць
//----------------------------------------------------------------------
int PoolPass::GetMonthsVisits() const
{
  return m_MonthsVisits;
}

//Гетер, який повертає тривалість перебування в день
//----------------------------------------------------------------------
int PoolPass::GetSessionDuration() const
{
  return m_SessionDuration;
}

//Гетер, який повертає період дії картки
//----------------------------------------------------------------------
int PoolPass::GetCardPeriod() const
{
  return m_CardPeriod;
}

//Гетер, який повертає прізвище, ім'я, по-батькові
//----------------------------------------------------------------------
QString PoolPass::GetName() const
{
  QString Temp;
  for(int i = 0; i < m_Name.size(); i++)
    Temp.append(m_Name.at(i) + ' ');
  return Temp;
}

//Сетер, який встановлює додаткові послуги
//----------------------------------------------------------------------
void PoolPass::SetAdditionaServices(QStringList AdditionalServices)
{
  m_AdditionalServices = AdditionalServices;
}

//Сетер, який встановлює сімейний тариф
//----------------------------------------------------------------------
void PoolPass::SetFamilyPlan(bool FamilyPlan)
{
  m_Family = FamilyPlan;
}

//Сетер, який встановлює к-сть відвідувань за місяць
//----------------------------------------------------------------------
void PoolPass::SetMonthsVisits(int MonthsVisits)
{
  m_MonthsVisits = MonthsVisits;
}

//Сетер, який встановлює тривалість перебування за день
//----------------------------------------------------------------------
void PoolPass::SetSessionDuration(int SessionDuration)
{
  m_SessionDuration = SessionDuration;
}

//Сетер, який встановлює період дії картки
//----------------------------------------------------------------------
void PoolPass::SetCardPeriod(int CardPeriod)
{
  m_CardPeriod = CardPeriod;
}

//Сетер, який встановлює прізвище, ім'я, по-батькові
//----------------------------------------------------------------------
void PoolPass::SetName(QStringList Names)
{
  m_Name = Names;
}

//Метод для очищення стрічок
//----------------------------------------------------------------------
void PoolPass::Clear()
{
  this->m_AdditionalServices.clear();
  this->m_Name.clear();
}

//Метод, який порівнює шукане значення з полями об'єкту
//Якщо хоча б одне значення співпадає вертаю true
bool PoolPass::SearchToCompare(const QString *SearchedString,
                               const int *SearchedSessionDur,
                               const int *SearchedMonthsVisits)
{
  bool ReturnValue = false;
  if(SearchedString)
    {
      if(this->GetName().contains(*SearchedString))
        ReturnValue = true;
    }
  if(SearchedSessionDur)
    {
      if(this->GetSessionDuration() == *SearchedSessionDur)
        ReturnValue = true;
    }
  if(SearchedMonthsVisits)
    {
      if(this->GetMonthsVisits() == *SearchedMonthsVisits)
        ReturnValue = true;
    }
  return ReturnValue;
}

//Перевантажений оператор виводу у файл
//----------------------------------------------------------------------
QTextStream & operator <<(QTextStream & Out, const PoolPass &rOther)
{
  Out << "Додаткові послуги:";
  for(int i = 0; i < rOther.m_AdditionalServices.size(); i++)
    Out << ' ' << rOther.m_AdditionalServices[i] << ',';
  Out << '\n';
  if(rOther.m_Family)
    Out << "Сімейний тариф: " << "присутній\n";
  if(!rOther.m_Family)
    Out << "Сімейний тариф: " << "відсутній\n";
  Out << "Кількість відвідувань за місяць: " << rOther.m_MonthsVisits << '\n';
  Out << "Тривалість перебування в день: " << rOther.m_SessionDuration << '\n';
  Out << "Період дії картки: " << rOther.m_CardPeriod << '\n';
  Out << "Прізвище, Ім'я, По-батькові:";
  for(int i = 0; i < rOther.m_Name.size(); i++)
    Out << ' ' << rOther.m_Name[i];
  Out << '\n';
  return Out;
}

//Додаткова функція, яка зчитує рядок з файлу, розділяє за символами
//':' та ' ' на окремі стрічки і не важливу частину вилучає
//----------------------------------------------------------------------
void ReadLine(QTextStream & In, QStringList & Temp)
{
  Temp = In.readLine().split(": ",
                             Qt::SkipEmptyParts,
                             Qt::CaseInsensitive);
  try
  {
    if(Temp.size() == 0)
      throw 0;
  }
  catch (int)
  {
    QMessageBox Box;
    Box.critical(NULL, "Абонемент в басейн", "Файл пошкоджений. "
"Робота додатку зупиняється.");
    exit(1);
  }
  Temp.pop_front();
}

//Перевантажений оператор вводу з файлу
//----------------------------------------------------------------------
QTextStream & operator >>(QTextStream & In, PoolPass &rOther)
{
  QStringList Temp;
  ReadLine(In, Temp);
  Temp = Temp.last().split(", ",
                           Qt::SkipEmptyParts,
                           Qt::CaseInsensitive);
  for(int i = 0; i < Temp.size(); i++)
    rOther.m_AdditionalServices.append(Temp.at(i));
  ReadLine(In, Temp);
  if(Temp.last().contains("п"))
    rOther.m_Family = true;
  if(Temp.last().contains("в"))
    rOther.m_Family = false;
  ReadLine(In, Temp);
  rOther.m_MonthsVisits = Temp.last().toInt();
  ReadLine(In, Temp);
  rOther.m_SessionDuration = Temp.last().toInt();
  ReadLine(In,Temp);
  rOther.m_CardPeriod = Temp.last().toInt();
  ReadLine(In, Temp);
  Temp = Temp.last().split(" ",
                           Qt::SkipEmptyParts,
                           Qt::CaseInsensitive);
  for(int i = 0; i < Temp.size(); i++)
    rOther.m_Name.append(Temp.at(i));
  return In;
}
