#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect 
{
  public:
    CRect(double x, double y, double w, double h )
      : m_X ( x ), m_Y ( y ), m_W ( w ), m_H ( h )
    {}

    friend ostream& operator <<(ostream& os,const CRect& x)
    {
      return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }

    double m_X;
    double m_Y;
    double m_W;
    double m_H;
};

#endif /* __PROGTEST__ */


struct CGuiPart
{
  CGuiPart(int id, const CRect& relPos)
    : m_Id(id), m_AbsPos(relPos), m_RelPos(relPos)
  {}
  //-------------------------------------------------------
  virtual ~CGuiPart(){};
  //-------------------------------------------------------
  virtual void print(ostream&) const = 0;
  //-------------------------------------------------------
  virtual void printLast(ostream&) const = 0;
  //-------------------------------------------------------
  virtual void printSearch(ostream&) const = 0;
  //-------------------------------------------------------
  virtual CGuiPart * clone() const = 0;
  //-------------------------------------------------------
  void resize(const CRect& x)
    {
      m_AbsPos.m_X = m_RelPos.m_X * x.m_W + x.m_X;
      m_AbsPos.m_Y = m_RelPos.m_Y * x.m_H + x.m_Y;
      m_AbsPos.m_W = x.m_W * m_RelPos.m_W;
      m_AbsPos.m_H = x.m_H * m_RelPos.m_H;
    }
  //-------------------------------------------------------
  int getId() const{return m_Id;}
  //-------------------------------------------------------
  int   m_Id;
  CRect m_AbsPos;
  CRect m_RelPos;
};

ostream& operator<<(ostream& os, const CGuiPart& x)
{
    x.printSearch(os);
    return os;
}
//======================================================================================================================

class CButton : public CGuiPart
{
public:
  CButton(int id, const CRect& relPos, const string& name)
    : CGuiPart(id, relPos), m_name(name)
    {}
  //--------------------------------------------------------------------------------------------------------------------
  void print(ostream& os) const override
  {
    os << "+- [" << m_Id << "] Button \"" << m_name << "\" " << m_AbsPos << '\n';
  }
  //--------------------------------------------------------------------------------------------------------------------
  void printLast(ostream& os) const override
    {
      os << "+- [" << m_Id << "] Button \"" << m_name << "\" " << m_AbsPos << '\n';
    }
  //--------------------------------------------------------------------------------------------------------------------
  void printSearch(ostream& os) const override
  {
    print(os);
  }
  //--------------------------------------------------------------------------------------------------------------------
  CButton * clone() const override { return new CButton(*this); }
  //--------------------------------------------------------------------------------------------------------------------
private:
    string  m_name;
};
//======================================================================================================================

class CLabel : public CGuiPart
{
public:
  CLabel(int id, const CRect& relPos, const string& label)
    : CGuiPart(id, relPos), m_name(label)
  {}
  //--------------------------------------------------------------------------------------------------------------------
  void print(ostream& os) const override
  {
    os << "+- [" << m_Id << "] Label \"" << m_name << "\" " << m_AbsPos << '\n';
  }
  //--------------------------------------------------------------------------------------------------------------------
  void printLast(ostream& os) const override
  {
    os << "+- [" << m_Id << "] Label \"" << m_name << "\" " << m_AbsPos << '\n';
  }
  //--------------------------------------------------------------------------------------------------------------------
  void printSearch(ostream& os) const override
  {
    print(os);
  }
  //--------------------------------------------------------------------------------------------------------------------
  CLabel * clone() const override { return new CLabel(*this); }
  //--------------------------------------------------------------------------------------------------------------------
private:
  string  m_name;
};
//======================================================================================================================

class CInput : public CGuiPart
{
public:
  CInput(int id,const CRect& relPos, const string& value)
    : CGuiPart(id, relPos), m_name(value)
  {}
  //--------------------------------------------------------------------------------------------------------------------
  void print(ostream& os) const override
  {
    os << "+- [" << m_Id << "] Input \"" << m_name << "\" " << m_AbsPos << '\n';
  }
  //--------------------------------------------------------------------------------------------------------------------
  void printLast(ostream& os) const override
  {
    os << "+- [" << m_Id << "] Input \"" << m_name << "\" " << m_AbsPos << '\n';
  }
  //--------------------------------------------------------------------------------------------------------------------
  void printSearch(ostream& os) const override
  {
    print(os);
  }
  //--------------------------------------------------------------------------------------------------------------------
  CInput * clone() const override { return new CInput(*this); }
  //--------------------------------------------------------------------------------------------------------------------
  void setValue(const string& x) { m_name = x;}
  //--------------------------------------------------------------------------------------------------------------------
  string getValue() const { return m_name;}
  //--------------------------------------------------------------------------------------------------------------------
private:
  string  m_name;
};
//======================================================================================================================

class CComboBox : public CGuiPart
{
public:
  CComboBox(int id, const CRect& relPos)
    : CGuiPart(id, relPos), m_selected(0)
  {}
  //--------------------------------------------------------------------------------------------------------------------
  void print(ostream& os) const override
  {
    os << "+- [" << m_Id << "] ComboBox " << m_AbsPos << "\n";
    for(size_t i = 0; i < m_options.size(); i++)
    {
      if(i == m_selected)
        os << "|  +->" << m_options[i] << "<\n";
      else
        os << "|  +- " << m_options[i] << "\n";
    }
  }
  //--------------------------------------------------------------------------------------------------------------------
  void printLast(ostream& os) const override
  {
    os << "+- [" << m_Id << "] ComboBox " << m_AbsPos << "\n";
    for(size_t i = 0; i < m_options.size(); i++)
    {
      if(i == m_selected)
        os << "   +->" << m_options[i] << "<\n";
      else
        os << "   +- " << m_options[i] << "\n";
    }
  }
  //--------------------------------------------------------------------------------------------------------------------
  void printSearch(ostream& os) const override
  {
    os << "[" << m_Id << "] ComboBox " << m_AbsPos << "\n";
    for(size_t i = 0; i < m_options.size(); i++)
    {
      if(i == m_selected)
        os << "+->" << m_options[i] << "<\n";
      else
        os << "+- " << m_options[i] << "\n";
    }
  }
  //--------------------------------------------------------------------------------------------------------------------
  CComboBox * clone() const override { return new CComboBox(*this); }
  //--------------------------------------------------------------------------------------------------------------------
  CComboBox& add(const string& x)
  {
    m_options.push_back(x);
    return *this;
  }
  //--------------------------------------------------------------------------------------------------------------------
  void setSelected(size_t x){m_selected = x;}
  //--------------------------------------------------------------------------------------------------------------------
  [[nodiscard]] size_t getSelected() const{return m_selected;}
  //--------------------------------------------------------------------------------------------------------------------
  vector<string> getOptions() {return m_options;}
  //--------------------------------------------------------------------------------------------------------------------
private:
  vector<string> m_options;
  size_t m_selected;
};
//======================================================================================================================

class CWindow 
{
public:
  CWindow(int id, const string& title, const CRect& absPos)
    : m_Id(id), m_Title(title), m_Pos(absPos)
  {}
  //--------------------------------------------------------------------------------------------------------------------
  CWindow(const CWindow& x) // Important .. Copy constructor
    : m_Id(x.m_Id), m_Title(x.m_Title), m_Pos(x.m_Pos)
  {
    for(auto it = x.m_Parts.begin(); it != x.m_Parts.end(); ++it)
      m_Parts.push_back((*it)->clone());
  }
  //--------------------------------------------------------------------------------------------------------------------
  CWindow& operator=(const CWindow& x)
  {
    if(this == &x) {return *this;}

    m_Id = x.m_Id;
    m_Pos = x.m_Pos;
    m_Title = x.m_Title;

    for (auto it = m_Parts.begin(); it != m_Parts.end(); ++it)
      delete *it;
    m_Parts.clear();

    for (auto it = x.m_Parts.begin(); it != x.m_Parts.end(); ++it)
      m_Parts.push_back((*it)->clone());

    return *this;
  }
  //--------------------------------------------------------------------------------------------------------------------
  ~CWindow()
  {
    for(size_t i = 0; i < m_Parts.size(); i++)
      delete m_Parts[i];
  }
  //--------------------------------------------------------------------------------------------------------------------
  CWindow& add(const CGuiPart& x)
  {
		CGuiPart* tmp = x.clone();
		tmp->resize(m_Pos);
    m_Parts.push_back(tmp);
    return *this;
  }
  //--------------------------------------------------------------------------------------------------------------------
  friend ostream& operator<<(ostream& os, const CWindow& x)
  {
    os << "[" << x.m_Id << "]" << " Window \"" << x.m_Title << "\" " << x.m_Pos << "\n";
    for(auto it = x.m_Parts.begin(); it != x.m_Parts.end(); ++it )
    {
      if(*it == x.m_Parts.back()) // last in the row
        (*it)->printLast(os);
      else // in the middle of the row
        (*it)->print(os);
    }
    return os;
  }
  //--------------------------------------------------------------------------------------------------------------------
  CGuiPart* search(int id)
  {
    for(auto it = m_Parts.begin(); it != m_Parts.end(); ++it)
    {
      if((*it) ->getId() == id)
        return *it;
    }
    return nullptr;
  }
  //--------------------------------------------------------------------------------------------------------------------
  void setPosition(CRect absPos)
  {
    m_Pos = absPos;
    for(auto it = m_Parts.begin(); it != m_Parts.end(); ++it)
      (*it)->resize(m_Pos);
  }
  //--------------------------------------------------------------------------------------------------------------------
private:
    int m_Id;
    string m_Title;
    CRect m_Pos;
    vector<CGuiPart*> m_Parts;
};
//======================================================================================================================


#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ( void )
{
  // My tests
  CWindow win(5, "MyWin", CRect(10,10,500,500));
  win.add(CLabel(1,CRect(0.1,0.1,0.1,0.1),"Hello"));
  win.add(CComboBox(5,CRect(0.5,0.1,0.1,0.1)).add("JiuJitsu").add("Progtest"));
  cout << win << endl;

  CWindow los(10,"MyLos",CRect(10,10,1000,2000));
  los.add(CComboBox(5,CRect(0.5,0.1,0.1,0.1)).add("Hi").add("Bye"));
  cout << los << endl;

  win = los;
  cout << win;




  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );

  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  CWindow b = a;
  assert ( toString ( *b . search ( 20 ) ) ==
    "[20] ComboBox (70,154,480,48)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
  b . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (70,250,480,48)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  b . setPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [20] ComboBox (84,186,512,52)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (84,290,512,52)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
