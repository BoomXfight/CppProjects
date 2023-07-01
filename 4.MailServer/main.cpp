#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */


template <class T>
class CVector
{
public:
	CVector() : m_Cap(5), m_Len(0), m_Vector(new T[m_Cap]) // Implicit Constructor
	{
	}

	CVector(int capacity) : m_Cap(capacity), m_Len(0), m_Vector(new T[m_Cap]) // Constructor
	{}

	CVector(const CVector<T>& x) : m_Cap(x.m_Cap), m_Len(x.m_Len), m_Vector(new T[m_Cap]) // Copy constructor
	{
		for(int i = 0; i < m_Len; i++)
			m_Vector[i] = x.m_Vector[i];
	}

	CVector<T>& operator=(const CVector<T>& x) // Assignment operator
	{
		if(this == &x) return *this;
		delete [] m_Vector;
		m_Cap = x.m_Cap;
		m_Len = x.m_Len;
		m_Vector = new T[m_Cap];
		for(int i = 0; i < m_Len; i++) // memory copy
			m_Vector[i] = x.m_Vector[i];
		return *this;
	}

	~CVector()
	{
		delete [] m_Vector;
	}

	void pushback(const T &x) // Pushback method - adds after the current last element
	{
		if(m_Len >= m_Cap)
			reallocate();

		m_Vector[m_Len] = x;
		m_Len ++;
	}

	bool contains(const T& x) // Method that finds out whether a vector contains the element x
	{
		for(int i = 0; i < m_Len; i++)
			if(x == m_Vector[i])
				return true;
		return false;
	}

	T& operator [] (int i) // Overload operator []
	{
		if (i<0 || i >= m_Len) throw "Index out of range";
		return(m_Vector[i]);
	}

	const T& operator [] (int i) const // Overload operator []
	{
		if (i<0 || i >= m_Len) throw "Index out of range";
		return(m_Vector[i]);
	}

	int getLen() const {return m_Len;} // Len getter

	friend ostream& operator<<(ostream& os, CVector<T>& x) // Friend function overloading output <<
	{
		for(int i = 0; i < x.m_Len; i++)
			os << "|" << x.m_Vector[i] << "|";
		return os;
	}

  bool operator==(const CVector<T> &o) const
  {
    if(m_Len != o.m_Len)
      return false;
    for(int i = 0; i < m_Len; i++)
      if(!(m_Vector[i] == o.m_Vector[i]))
        return false;
    return true;
  }

private:
	int m_Cap;
	int m_Len;
	T * m_Vector;

	void reallocate()
	{
		CVector tmp = *this;
		delete [] m_Vector;
		m_Cap *= 2;
		m_Vector = new T[m_Cap];
		for(int i = 0; i < m_Len; i++)
			m_Vector[i] = tmp.m_Vector[i];
	}
};

class CString
{
public:
	CString() : m_Len(1), m_Cap(1), m_Data(new char[m_Cap]) // Implicit constructor
	{
		m_Data[0] = '\0';
	}

	CString(const char * str) : m_Len(strlen(str)), m_Cap(m_Len + 1), m_Data(new char[m_Cap]) // C string constructor
	{
		memcpy(m_Data, str, m_Cap);
	}

	CString(const CString& x) : m_Len(x.m_Len), m_Cap(x.m_Cap), m_Data(new char[m_Cap]) // Copy constructor
	{
		memcpy(m_Data, x.m_Data, m_Cap);
	}

	~CString() // Destructor
	{
		delete [] m_Data;
	};

	CString& operator=(const CString& x) // Assign operator
	{
		if(this == &x) return *this;
		delete [] m_Data;

		m_Cap = x.m_Cap;
		m_Len = x.m_Len;
		m_Data = new char[m_Cap];

		memcpy(m_Data, x.m_Data, m_Cap);
		return *this;
	}

	friend ostream& operator<<(ostream& os, const CString &x) // Output operator <<
	{
		os << x.m_Data;
		return os;
	}


	bool operator==(const CString &x) const // Operator == comparing 2 strings
	{
		if (m_Cap != x.m_Cap || m_Len != x.m_Len)
			return false;
		for(size_t i = 0; i < m_Len; i++)
			if(m_Data[i] != x.m_Data[i])
				return false;
		return true;
	}

private:
	size_t m_Len;
	size_t m_Cap;
	char * m_Data;
};

class CMail
{
public:
  CMail() = default;

	CMail(const char * from, const char * to, const char * body) // Constructor
    : m_From(from), m_To(to), m_Body(body)
  {}

	bool operator ==(const CMail & x) const // Comparison operator ==
  {
    return(m_From == x.m_From && m_To == x.m_To && m_Body == x.m_Body);
  }

	friend ostream& operator <<(ostream& os,const CMail & m) // Overloads ostream <<
  {
    os << "From: " << m.m_From << ", To: " << m.m_To << ", Body: " << m.m_Body;
    return os;
  }

  CString getFrom() const {return m_From;}
  CString getTo() const {return  m_To;}

private:
	CString m_From;
  CString m_To;
  CString m_Body;
};

class CMailAddress
{
public:
  CMailAddress() = default; // Constructor

  CMailAddress(const CString &name) : m_Name(name) // Constructor with parameter
  {}

  void inboxPush(const CMail &mail)
  {
    m_Inbox.pushback(mail);
  }

  void outboxPush(const CMail &mail)
  {
    m_Outbox.pushback(mail);
  }

  CString getName() const {return m_Name;}

  friend class CMailServer;

private:
  CString m_Name;
  CVector<CMail> m_Inbox;
  CVector<CMail> m_Outbox;
};

class CMailIterator
{
public:
  CMailIterator() : m_Box(), m_Position(0)
  {}

  CMailIterator(const CVector<CMail> &box) : m_Box(box), m_Position(0)
  {}

	explicit operator bool(void) const
  {
    return m_Position < m_Box.getLen();
  }

	bool operator !(void) const
  {
    return m_Position >= m_Box.getLen();
  }

	const CMail& operator *(void)const
  {
    return(m_Box[m_Position]);
  }

	CMailIterator& operator ++(void)
  {
    m_Position ++;
    return *this;
  }

  friend class CMailServer;

private:
  CVector<CMail> m_Box;
	int m_Position;
};

class CMailServer
{
public:
	CMailServer () = default; // Default constructor

	CMailServer (const CMailServer& src) : m_Database(src.m_Database) // Copy constructor
  {}

	CMailServer& operator=(const CMailServer& src) = default;// Assignment operator =

	~CMailServer () = default; // Destructor

	void sendMail(const CMail& m)
  {
    int flagFrom = -1;
    int flagTo = -1;
    for(int i = 0; i < m_Database.getLen(); i++) // Look for the MailAddress in the database
    {
      if(m_Database[i].getName() == m.getFrom())
        flagFrom = i;
      if(m_Database[i].getName() == m.getTo())
        flagTo = i;
    }

    if(flagFrom != -1) // Send the mail
      m_Database[flagFrom].outboxPush(m);
    else
    {
      m_Database.pushback(CMailAddress(m.getFrom()));
      m_Database[m_Database.getLen() - 1].outboxPush(m);
    }

    if(flagTo != -1) // Send the mail
      m_Database[flagTo].inboxPush(m);
    else
    {
      m_Database.pushback(CMailAddress(m.getTo()));
      m_Database[m_Database.getLen() - 1].inboxPush(m);
    }
  }

	CMailIterator outbox (const char * email) const
  {
    int postition = -1;
    for(int i = 0; i < m_Database.getLen(); i++)
      if(m_Database[i].getName() == email)
        postition = i;

    if(postition != -1)
      return CMailIterator(m_Database[postition].m_Outbox);
    else
      return CMailIterator();
  }

	CMailIterator inbox (const char * email) const
  {
    int postition = -1;
    for(int i = 0; i < m_Database.getLen(); i++)
      if(m_Database[i].getName() == email)
        postition = i;

    if(postition != -1)
      return CMailIterator(m_Database[postition].m_Inbox);
    else
      return CMailIterator();
  }

private:
	CVector<CMailAddress> m_Database;
};

#ifndef __PROGTEST__
bool matchOutput(const CMail& m, const char* str)
{
	ostringstream oss;
	oss << m;
	return oss.str() == str;
}
int main ( void )
{
	// Testing string
  /*
	CString a("Ahoj"), b = "Cauko", c = a;
	cout << "a:" << a << endl;
	cout << "b:" << b << endl;
	cout << "c:" << c << endl;
	CString d(a);
	cout << "d:" << d << endl;
	CString e(CString("Baby"));
	cout << "e:" << e << endl;
  cout << boolalpha << (a == b) << endl;
  cout << (a == c) << endl;
  */

	// Testing Vector
  /*
	CVector<int> numbers;
	numbers.pushback(10);
	numbers.pushback(20);
	numbers.pushback(30);
	numbers.pushback(153);
	cout << numbers << endl;

	CVector<CString> words;
	words.pushback(CString("Ahoj"));
	words.pushback("Aho?");
	cout << words << endl;

	CVector<CString> anotherWords(words);
	cout << anotherWords << endl;
  anotherWords.pushback("bro");
  anotherWords.pushback("bro");
  anotherWords.pushback("bro");
  anotherWords.pushback("bro");
  cout << anotherWords << endl;

  words = anotherWords;
  cout << words << endl;
  cout << words[5] << endl;

  CVector<CMail> mails, emails;
  mails.pushback(CMail("Andrej", "Matej", "Hi."));
  mails.pushback(CMail("Andrej", "Matej", "Hi."));
  mails.pushback(CMail("Andrej", "Matej", "Hi."));
  mails.pushback(CMail("Andrej", "Matej", "Hi."));
  mails.pushback(CMail("Andrej", "Matej", "Hi."));
  mails.pushback(CMail("Andrej", "Matej", "Hi."));
  cout << mails << endl;
  emails = mails;
  cout << emails << endl;
  emails.pushback(CMail("Emil", "Tomas", "No"));
  emails.pushback(CMail("Emil", "Tomas", "No"));
  emails.pushback(CMail("Emil", "Tomas", "No"));
  cout << emails << endl;
  cout << !(emails == mails) << endl;
  */

  // Testing CMail
  /*
  CMail a("Andrej", "Matej", "Ahoj.");
  CMail b("Matej", "Andrej", "Hi.");
  CMail x("BU", "HU", "BE");
  cout << "a: " << a << endl;
  cout << "b: " << b << endl;
  cout << (a == b) << endl;
  CMail c(a);
  cout << "c: " <<c << endl;
  c = b;
  cout << "c: " << c << endl;
  cout << "a: " << a << endl;
  c = x;
  cout << "c: " << c << endl;
  cout << "b: " << b << endl;
  */

  // Testing CMailAddress
  /*
  CMailAddress first("andrej.becka20@gmail.com");
  CMailAddress second("matej.sedlak5@gmail.com");
  first.inboxPush(CMail("matej.sedlak5@gmail.com", "andrej.becka20@gmail.com", "Hi."));
  first.inboxPush(CMail("matej.sedlak5@gmail.com", "andrej.becka20@gmail.com", "Hi."));
  first.inboxPush(CMail("matej.sedlak5@gmail.com", "andrej.becka20@gmail.com", "Hi."));
  first.inboxPush(CMail("matej.sedlak5@gmail.com", "andrej.becka20@gmail.com", "Hi."));
  first.inboxPush(CMail("matej.sedlak5@gmail.com", "andrej.becka20@gmail.com", "Hi."));
  first.inboxPush(CMail("matej.sedlak5@gmail.com", "andrej.becka20@gmail.com", "Hi."));
  second.outboxPush(CMail("matej.sedlak5@gmail.com", "andrej.becka20@gmail.com", "Hi."));
  second.outboxPush(CMail("matej.sedlak5@gmail.com", "andrej.becka20@gmail.com", "Hi."));
  second.outboxPush(CMail("matej.sedlak5@gmail.com", "andrej.becka20@gmail.com", "Hi."));
  second.outboxPush(CMail("matej.sedlak5@gmail.com", "andrej.becka20@gmail.com", "Hi."));
  second.outboxPush(CMail("matej.sedlak5@gmail.com", "andrej.becka20@gmail.com", "Hi."));
  second.outboxPush(CMail("matej.sedlak5@gmail.com", "andrej.becka20@gmail.com", "Hi."));
  */

  // Testing CMailServer
  /*
  CMailServer s1;
  s1.sendMail(CMail("andrej.becka30@gmail.com", "matej.sedlak5@gmail.com", "ahoj"));
  s1.sendMail(CMail("matej.sedlak5@gmail.com", "andrej.becka30@gmail.com", "ahoj!"));
  s1.sendMail(CMail("andrej.becka30@gmail.com", "matej5@gmail.com", "ahojky"));
  s1.sendMail(CMail("andrej.becka30@gmail.com", "matej5@gmail.com", "Ako sa mas?"));
  s1.sendMail(CMail("andrej.becka30@gmail.com", "matej5@gmail.com", "Wanna go out?"));
  */

  // My tests
  CMailServer server;
  server.sendMail(CMail("Andrej", "Matej", "Hi"));
  server.sendMail(CMail("Andrej", "Tomas", "Hi"));
  server.sendMail(CMail("Andrej", "Lada", "Hi"));
  server.sendMail(CMail("Andrej", "Monika", "Hi"));
  CMailIterator iter0 = server.outbox("Andrej");
  cout << *iter0 << endl;
  ++iter0;
  if(iter0)
    cout << *iter0 << endl;
  ++iter0;
  if(iter0)
    cout << *iter0 << endl;
  ++iter0;
  if(iter0)
    cout << *iter0 << endl;
  ++iter0;
  if(iter0)
    cout << *iter0 << endl;
  ++iter0;
  if(iter0)
    cout << *iter0 << endl;




	char from[100], to[100], body[1024];

	assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
	assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
	assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
	assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
	assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
	assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );
	CMailServer s0;
	s0 . sendMail ( CMail ( "john", "peter", "some important mail" ) );
	strncpy ( from, "john", sizeof ( from ) );
	strncpy ( to, "thomas", sizeof ( to ) );
	strncpy ( body, "another important mail", sizeof ( body ) );
	s0 . sendMail ( CMail ( from, to, body ) );
	strncpy ( from, "john", sizeof ( from ) );
	strncpy ( to, "alice", sizeof ( to ) );
	strncpy ( body, "deadline notice", sizeof ( body ) );
	s0 . sendMail ( CMail ( from, to, body ) );
	s0 . sendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
	s0 . sendMail ( CMail ( "peter", "alice", "PR bullshit" ) );

	CMailIterator i0 = s0 . inbox ( "alice" );
	assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );
	assert ( matchOutput ( *i0,  "From: john, To: alice, Body: deadline notice" ) );
	assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
	assert ( matchOutput ( *i0,  "From: peter, To: alice, Body: PR bullshit" ) );
	assert ( ! ++i0 );

	CMailIterator i1 = s0 . inbox ( "john" );
	assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
	assert ( matchOutput ( *i1,  "From: alice, To: john, Body: deadline confirmation" ) );
	assert ( ! ++i1 );

	CMailIterator i2 = s0 . outbox ( "john" );
	assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
	assert ( matchOutput ( *i2,  "From: john, To: peter, Body: some important mail" ) );
	assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
	assert ( matchOutput ( *i2,  "From: john, To: thomas, Body: another important mail" ) );
	assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
	assert ( matchOutput ( *i2,  "From: john, To: alice, Body: deadline notice" ) );
	assert ( ! ++i2 );

	CMailIterator i3 = s0 . outbox ( "thomas" );
	assert ( ! i3 );

	CMailIterator i4 = s0 . outbox ( "steve" );
	assert ( ! i4 );

	CMailIterator i5 = s0 . outbox ( "thomas" );
	s0 . sendMail ( CMail ( "thomas", "boss", "daily report" ) );
	assert ( ! i5 );

	CMailIterator i6 = s0 . outbox ( "thomas" );
	assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
	assert ( matchOutput ( *i6,  "From: thomas, To: boss, Body: daily report" ) );
	assert ( ! ++i6 );

	CMailIterator i7 = s0 . inbox ( "alice" );
	s0 . sendMail ( CMail ( "thomas", "alice", "meeting details" ) );
	assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
	assert ( matchOutput ( *i7,  "From: john, To: alice, Body: deadline notice" ) );
	assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
	assert ( matchOutput ( *i7,  "From: peter, To: alice, Body: PR bullshit" ) );
	assert ( ! ++i7 );

	CMailIterator i8 = s0 . inbox ( "alice" );
	assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
	assert ( matchOutput ( *i8,  "From: john, To: alice, Body: deadline notice" ) );
	assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
	assert ( matchOutput ( *i8,  "From: peter, To: alice, Body: PR bullshit" ) );
	assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
	assert ( matchOutput ( *i8,  "From: thomas, To: alice, Body: meeting details" ) );
	assert ( ! ++i8 );

	CMailServer s1 ( s0 );
	s0 . sendMail ( CMail ( "joe", "alice", "delivery details" ) );
	s1 . sendMail ( CMail ( "sam", "alice", "order confirmation" ) );
	CMailIterator i9 = s0 . inbox ( "alice" );
	assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
	assert ( matchOutput ( *i9,  "From: john, To: alice, Body: deadline notice" ) );
	assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
	assert ( matchOutput ( *i9,  "From: peter, To: alice, Body: PR bullshit" ) );
	assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
	assert ( matchOutput ( *i9,  "From: thomas, To: alice, Body: meeting details" ) );
	assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
	assert ( matchOutput ( *i9,  "From: joe, To: alice, Body: delivery details" ) );
	assert ( ! ++i9 );

	CMailIterator i10 = s1 . inbox ( "alice" );
	assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
	assert ( matchOutput ( *i10,  "From: john, To: alice, Body: deadline notice" ) );
	assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
	assert ( matchOutput ( *i10,  "From: peter, To: alice, Body: PR bullshit" ) );
	assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
	assert ( matchOutput ( *i10,  "From: thomas, To: alice, Body: meeting details" ) );
	assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
	assert ( matchOutput ( *i10,  "From: sam, To: alice, Body: order confirmation" ) );
	assert ( ! ++i10 );

	CMailServer s2;
	s2 . sendMail ( CMail ( "alice", "alice", "mailbox test" ) );
	CMailIterator i11 = s2 . inbox ( "alice" );
	assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
	assert ( matchOutput ( *i11,  "From: alice, To: alice, Body: mailbox test" ) );
	assert ( ! ++i11 );

	s2 = s0;
	s0 . sendMail ( CMail ( "steve", "alice", "newsletter" ) );
	s2 . sendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
	CMailIterator i12 = s0 . inbox ( "alice" );
	assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
	assert ( matchOutput ( *i12,  "From: john, To: alice, Body: deadline notice" ) );
	assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
	assert ( matchOutput ( *i12,  "From: peter, To: alice, Body: PR bullshit" ) );
	assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
	assert ( matchOutput ( *i12,  "From: thomas, To: alice, Body: meeting details" ) );
	assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
	assert ( matchOutput ( *i12,  "From: joe, To: alice, Body: delivery details" ) );
	assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
	assert ( matchOutput ( *i12,  "From: steve, To: alice, Body: newsletter" ) );
	assert ( ! ++i12 );

	CMailIterator i13 = s2 . inbox ( "alice" );
	assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
	assert ( matchOutput ( *i13,  "From: john, To: alice, Body: deadline notice" ) );
	assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
	assert ( matchOutput ( *i13,  "From: peter, To: alice, Body: PR bullshit" ) );
	assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
	assert ( matchOutput ( *i13,  "From: thomas, To: alice, Body: meeting details" ) );
	assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
	assert ( matchOutput ( *i13,  "From: joe, To: alice, Body: delivery details" ) );
	assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
	assert ( matchOutput ( *i13,  "From: paul, To: alice, Body: invalid invoice" ) );
	assert ( ! ++i13 );

	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
