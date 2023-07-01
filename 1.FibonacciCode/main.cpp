#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
#endif /* __PROGTEST__ */


/**
 * This function reads 1 unicode character from file
 * @param[in] file_name name of the file
 * @param[in] from position from which file should be read
 * @param[out] value unicode value of byte sequence
 * @return 1 = succes, 2 = open failure, 3 = read failure, 4 = false utf coding
*/
size_t read_byte ( size_t & from, ifstream & in, unsigned int & value, vector <char> & buff )
{
  buff.clear ();
  char buffer[4];

  in.seekg( from );
  if ( ! in.read ( buffer, 1 ) )
    return 3;

  value = 0;
  buff.push_back ( buffer[0] );
  unsigned char first_byte = static_cast <unsigned char> ( buffer[0] );

  if ( first_byte <= 127 ) // 1 byte = 1 unicode char
  {
    value = first_byte;
    from += 1;
  }

  else if ( first_byte >= 192 && first_byte <= 223 ) // 2 bytes = 1 unicode char
  {
    if ( ! in.read( buffer + 1, 1 ) )
      return 3;
    buff.push_back ( buffer[1] );

    unsigned char second_byte = static_cast <unsigned char> ( buffer[1] );
    if ( ( second_byte & 0xc0 ) != 128 ) // check invalid follow byte
      return 4;

    value = ( ( first_byte & 0x1f ) << 6 ) | ( second_byte & 0x3f );
    from += 2;
  }

  else if ( first_byte >= 224 && first_byte <= 239 ) // 3 bytes = 1 unicode char
  {
    if ( ! in.read( buffer + 1, 1 ) )
      return 3;
    buff.push_back ( buffer[1] );

    if ( ! in.read ( buffer + 2, 1 ) )
      return 3;

    unsigned char second_byte = static_cast <unsigned char> ( buffer[1] );
    unsigned char third_byte = static_cast <unsigned char> ( buffer[2] );
    if ( ( second_byte & 0xc0 ) != 128 || ( third_byte & 0xc0 ) != 128 ) // check invalid follow bytes
      return 4;

    value = ( ( first_byte & 0x0f ) << 12 ) | ( ( second_byte & 0x3f ) << 6 ) | ( third_byte & 0x3f );
    from += 3;
  }

  else if ( first_byte >= 240 && first_byte <= 247 ) { // 4 bytes = 1 unicode char
    if ( ! in.read( buffer + 1, 1 ) ) {
      return 3;
    }
    buff.push_back ( buffer[1] );
    if ( ! in.read( buffer + 2, 1 ) ) {
      return 3;
    }
    buff.push_back ( buffer[2] );
    if ( ! in.read( buffer + 3, 1 ) ) {
      return 3;
    }
    unsigned char second_byte = static_cast<unsigned char> ( buffer[1] );
    unsigned char third_byte = static_cast<unsigned char> ( buffer[2] );
    unsigned char fourth_byte = static_cast<unsigned char> ( buffer[3] );
    if ( ( second_byte & 0xc0) != 128 || ( third_byte & 0xc0 ) != 128 || ( fourth_byte & 0xc0 ) != 128 ) // check invalid follow bytes
      return 4;

    value = ( ( first_byte & 0x07 ) << 18 ) | ( ( second_byte & 0x3f ) << 12) | ( ( third_byte & 0x3f ) << 6) | ( fourth_byte & 0x3f );
    from += 4;
  }

  else  // invalid unicode character
    return 4;

  return 1;
}


/**
 * This function translates a vector of characters to fibbonaci code ( bit sequence )
 * 
*/
vector <char> int_to_fib ( int value, vector <int> & fibonacci_seq )
{
  vector <char> f_b; // fibbonaci_binary
  if ( value == 0 )
  {
    f_b = { '1', '1' };
    return f_b;
  }

  int begin = 0;
  for ( int i = fibonacci_seq.size()-1; i >= 0; i-- )
  {
    if ( value >= fibonacci_seq[i] )
    {
      f_b.insert( f_b.begin(), '1' );
      begin = 1;
      value -= fibonacci_seq[i];
    }
    else
      if ( begin == 1 )
        f_b.insert( f_b.begin(), '0' ); 
  }
  f_b.insert( f_b.end(), '1' );
  return f_b;
}


/**
 * This function writes a byte ( 8 bits of fibonacci ) into file
*/
bool write_byte ( vector <char> & v, ofstream & file, size_t size )
{
  while ( size >= 8 )
  {
    string byte;

    for ( int i = 7; i >= 0; i-- ) 
    {
      byte += v[i];
      v.erase ( v.begin() + i );
    }

    file.put ( stoi ( byte, nullptr, 2 ) );
    if ( ! file.good ( ) )
      return false;

    size -= 8;
  }
  return true;
}


/**
 * This function adds necessary zeros to the ending byte
*/
void add_zeros ( vector <char> & hl )
{
  vector <char> help;
  if ( hl.size() < 8 && hl.size() != 0 )
  {
    int amount = 8 - ( hl.size() % 8 );
    for ( int i = 0; i < amount; i++ )
      help.push_back ( '0' );
  }

  hl.insert ( hl.end(), help.begin(), help.end() );
}


vector <int> generate_fibbonaci_size ( size_t size ) // TESTED
{
  vector <int> fibonaci = { 1, 2 };
  fibonaci.reserve ( 30 );

  for ( size_t i = 2; i < size; i++ )
    fibonaci.push_back( fibonaci.back() + *( fibonaci.rbegin() + 1) );

  return fibonaci; 
}


bool utf8ToFibonacci ( const char * inFile, const char * outFile )
{
  ifstream in ( inFile, ios::binary );
  ofstream out ( outFile, ios::binary );
  if ( ! out.is_open () || out.fail () || ! in.is_open() || in.fail() )
    return false;

  size_t from = 0;
  unsigned int value = 0;

  vector <char> binary_fibonacci;
  binary_fibonacci.reserve ( 30 );
  vector <char> buff;
  buff.reserve ( 4 );

  vector <int> fibonacci_sequence;
  fibonacci_sequence . reserve ( 30 );
  fibonacci_sequence = generate_fibbonaci_size ( 30 );

  size_t status = read_byte ( from, in, value, buff );
  while ( status )
  {
    vector <char> b_f;
    if ( status == 4 || status == 2 ) return false;
    else if ( status == 3 ) // end of file
    {
      if ( buff.empty() == false )
        return false;

      add_zeros ( binary_fibonacci );
      if ( write_byte ( binary_fibonacci, out, binary_fibonacci.size() ) == false )
        return false;
      return true;
    }
    else
    {
      b_f = int_to_fib ( value == 0 ? value : value + 1, fibonacci_sequence );
      binary_fibonacci.insert ( binary_fibonacci.end(), b_f.begin(), b_f.end() );
    }
    if ( binary_fibonacci.size () >= 8 )
      if ( write_byte ( binary_fibonacci, out, binary_fibonacci.size() ) == false )
        return false;
    
    status = read_byte ( from, in, value, buff );
  }
  in.close( );
  out.close( );
  return true;
}
//---------------------------------------------------------------------------------------------


bool int_to_utf8 ( int value, ofstream & out )
{
  char utf8_buffer [4];
  unsigned int code = static_cast <unsigned int> ( value );

  // Convert the Unicode code point to a UTF-8 encoded byte sequence
  int byte_count = 0;
  if ( code <= 0x7F ) 
  {
    utf8_buffer[ byte_count ] = static_cast<char> ( code );
    byte_count ++;

  } 
  else if ( code <= 0x7FF ) 
  {
    utf8_buffer[ byte_count ] = static_cast<char> ( ( code >> 6 ) | 0xC0 );
    byte_count ++;
    utf8_buffer[ byte_count ] = static_cast<char> ( ( code & 0x3F) | 0x80 );
    byte_count ++;
  } 
  else if (code <= 0xFFFF) 
  {
    utf8_buffer [ byte_count ] = static_cast<char> ( ( code >> 12 ) | 0xE0 );
    byte_count ++;
    utf8_buffer[ byte_count ] = static_cast<char>( ( ( code >> 6 ) & 0x3F ) | 0x80 );
    byte_count ++;
    utf8_buffer[ byte_count ] = static_cast<char> ( ( code & 0x3F ) | 0x80 );
    byte_count ++;
  } 
  else if ( code <= 0x10FFFF ) 
  {
    utf8_buffer[ byte_count ] = static_cast<char> ( ( code >> 18 ) | 0xF0 );
    byte_count ++;
    utf8_buffer[ byte_count ] = static_cast<char> ( ( ( code >> 12 ) & 0x3F) | 0x80);
    byte_count ++;
    utf8_buffer[ byte_count ] = static_cast<char> ( ( ( code >> 6 ) & 0x3F) | 0x80);
    byte_count ++;
    utf8_buffer[ byte_count ] = static_cast<char> ( ( code & 0x3F ) | 0x80);
    byte_count ++;
  }

  for ( int i = 0; i < byte_count; i++ )
  {
    out.put ( utf8_buffer[i] );
    if ( ! out.good () )
      return false;
  }
  return true;
}


bool translate_to_fib ( vector <int> & bits, ofstream & file, vector <int> & fibonacci_sequence ) // TESTED
{
  int value = 0;

  if ( bits.size( ) == 1 
    && bits[0] == 1 )
    value = 0;

  else 
  {
    
    for ( size_t i = 0; i < bits.size ( ); i++ )
    {
      if ( bits[i] == 1 )
        value += fibonacci_sequence[i];
    }
    value -= 1;
  }

  if ( value > 1114111 ) // Invalid utf8 value
    return false;

  if ( int_to_utf8 ( value, file ) == false )
    return false;

  return true;
}


bool analyse_byte ( char byte, vector <int> & array, ofstream & file, int & one, vector <int> & fib_seq )
{
  vector <int> bits;
  bits.reserve ( 8 );

  for ( int i = 0; i < 8; i++ )
    bits.push_back ( ( byte >> i ) & 0x01 ); 

  
  for (  int i = 0; i < 8; i++ ) 
  {
    if ( bits[i] == 1 )
    {
      if ( one == 1 )
      {
        if ( translate_to_fib ( array, file, fib_seq ) == false )
          return false;

        array.clear( );
        one = 0;
      }
      else
      {
        one = 1;
        array.push_back( bits[i] );
      }
    }
    else
    {
      one = 0;
      array.push_back( bits[i] );
    } 
  }

  if ( array.size ( ) >= 30 ) // maximum size
    return false;

  return true;
}


bool fibonacciToUtf8 ( const char * inFile, const char * outFile ) 
{
  ifstream in ( inFile, ios::binary );
  ofstream out ( outFile, ios::binary );

  if ( ! in.is_open () || in.fail () || ! out.is_open () || out.fail () ) // could not open file;
    return false;

  char buffer[1];
  vector <int> bits_value;
  bits_value.reserve ( 30 );
  vector <int> fibonacci_sequence = generate_fibbonaci_size ( 30 );
  int one = 0;

  while ( in.read ( buffer, 1 ) ) 
  {
    if ( ! in.good ( ) )
      return false;

    if ( analyse_byte ( buffer[0], bits_value, out, one, fibonacci_sequence ) == false )
      return false;
  }

  if ( bits_value.size ( ) > 8 ) // checking if all sequences ended
    return false;
  
  for ( size_t i = 0; i < bits_value.size (); i++ )
    if ( bits_value[i] != 0 )
      return false;

  in.close ();
  out.close ();
  return true;
}


#ifndef __PROGTEST__


bool identicalFiles ( const char * file1, const char * file2 )
{
  ifstream f1 ( file1, ios::binary );
  ifstream f2 ( file2, ios::binary );

  if ( ! f1.is_open ( ) || ! f2.is_open ( ) )
    return false;

  bool equal = true;
  char c1, c2;
  while ( equal && f1.get ( c1 ) && f2.get ( c2 ) )
  {
    if ( c1 != c2 ) 
      equal = false;
  }

  f1.close ( );
  f2.close ( );

  return true;
}


int main ( int argc, char * argv [] )
{
  /* TEST generate_fibbonaci_size 
  vector <int> fibonacci;
  fibonacci = generate_fibbonaci_size ( 29 );
  for ( int i = 0; i < fibonacci.size ( ); i++ )
    cout << fibonacci[i] << " ";
  cout << endl;
  */
  /* TEST translate_to_fib
  vector <int> bits1 = { 1 };
  vector <int> bits2 = { 1, 0, 0, 1 };
  vector <int> bits3 = { 0, 0, 1, 0, 1 };
  translate_to_fib ( bits1 );
  translate_to_fib ( bits2 );
  translate_to_fib ( bits3 );
  */

  //utf8ToFibonacci ( "Napovedy/in_5026070.bin", "napoveda.out" );
  //if ( utf8ToFibonacci ( "UTF_TO_FIB/in_5026483.bin", "out_5026483" ) == false )
  //  return EXIT_FAILURE;
  //if ( utf8ToFibonacci ( "example/src_2.utf8", "output_src_2.fib" ) == false )
  //  return EXIT_FAILURE;
  
  /*
  utf8ToFibonacci ( "long_in_utf8", "long_out_fib" );
  utf8ToFibonacci ( "example/src_0.utf8", "output.fib0" );
  utf8ToFibonacci ( "example/src_1.utf8", "output.fib1" );
  utf8ToFibonacci ( "example/src_2.utf8", "output.fib2" );
  utf8ToFibonacci ( "example/src_3.utf8", "output.fib3" );
  utf8ToFibonacci ( "example/src_4.utf8", "output.fib4" );
  */
  
  assert ( utf8ToFibonacci ( "example/src_0.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_0.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_1.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_1.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_2.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_2.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_3.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_3.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_4.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_4.fib" ) );
  assert ( ! utf8ToFibonacci ( "example/src_5.utf8", "output.fib" ) );
  assert ( fibonacciToUtf8 ( "example/src_6.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_6.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_7.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_7.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_8.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_8.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_9.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_9.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_10.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_10.utf8" ) );
  assert ( ! fibonacciToUtf8 ( "example/src_11.fib", "output.utf8" ) );
  
  //-------------------------------------------------------------------
  assert ( ! utf8ToFibonacci ( "UTF_TO_FIB/in_1322558.bin", "out_1322558" ) );
  assert ( ! utf8ToFibonacci ( "UTF_TO_FIB/in_5026483.bin", "out_5026483" ) );
  

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
