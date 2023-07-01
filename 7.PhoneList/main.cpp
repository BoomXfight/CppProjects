#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */


struct SYellowBook
{
  string s_name;
  string s_surname;
  string s_number;
};

/**
 * This function checks whether the inputted number is has a valid formatting
 * @param [in] num
 * @return false -> not a correct formatting, true -> correct
*/
bool checkNum ( string & num )
{
  if ( num . length ( ) != 9 )    // controls the length of the string
    return false;
  if ( num.at(0) == '0' )            // check the first digit
    return false;
  for ( size_t i = 0; i < 9; i++ )
  { 
    if ( num[i] < 48 || num[i] > 57 )  // checks all numbers via ascii value
      return false;
  }
  return true;
}

bool parseLine ( string & line, SYellowBook & tmp )
{
  istringstream is ( line );

  if ( ! ( is >> tmp.s_name >> ws >> tmp.s_surname >> ws >> tmp.s_number )
      || checkNum ( tmp.s_number )  == false )
    return false;

  is >> ws;
  is . get ();
  return is . eof (); 
}

bool requestInBook ( string req, vector < SYellowBook> vec, ostream & o, size_t & from )
{
  for ( ; from < vec . size (); from++ )
  {
    if ( req == vec[from].s_name || req == vec[from].s_surname )
    {
      o << vec[from].s_name << " " << vec[from].s_surname << " " << vec[from].s_number << "\n";
      from ++;
      return true;
    }
  }
  return false;
}

bool checkReqest ( string & line, string & request )
{
  istringstream is ( line );
  is >> request;
  is >> ws;
  is . get ();
  return is . eof ();
}

bool report ( const string & fileName, ostream & out )
{
  vector < SYellowBook > v;
  ifstream ifs ( fileName );
  if ( ! ifs . is_open() )
    return false;


  string line;
  SYellowBook tmp;
  while ( getline ( ifs, line ) && !line . empty ( ) )
  {
    if ( parseLine ( line, tmp ) )
      v . push_back ( tmp );
    else
      return false;
  }
  
  string request;
  size_t from = 0;
  size_t cnt = 0;
  while ( getline ( ifs, line ) )                        // writng to the ostringstream
  {
    if ( ! checkReqest (line, request ) )
      return false;
    
    while ( requestInBook ( request, v, out, from ) )
      cnt ++;
    {
      out << "-> " << cnt << "\n";
      from = 0;
      cnt = 0;
    }
  } 
  return true;
}

#ifndef __PROGTEST__
int main ()
{
  ostringstream oss;
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss ) == true );
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    "-> 2\n"
    "-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    "-> 2\n"
    "John Harmson 861647702\n"
    "-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );
  oss . str ( "" );
  assert ( report( "tests/test2_in.txt", oss ) == false );
  oss . str ( "" );
  assert ( report( "tests/test3_in.txt", oss ) == false );
  oss . str ( "" );
  assert ( report( "tests/test4_in.txt", oss ) == false );
  oss . str ( "" );
  assert ( report( "tests/test5_in.txt", oss ) == false );
  oss . str ( "" );
  assert ( report( "tests/test6_in.txt", oss ) == false );
  oss . str ( "" );
  assert ( report( "tests/test7_in.txt", oss ) == false );
  oss . str ( "" );
  assert ( report( "tests/test8_in.txt", oss ) == true );
  oss . str ( "" );
  assert ( report( "tests/test9_in.txt", oss ) == false );
  oss . str ( "" );
  assert ( report( "tests/test10_in.txt", oss ) == false );
  oss . str ( "" );
  assert ( report( "tests/test11_in.txt", oss ) == false );
  return 0;
}
#endif /* __PROGTEST__ */
