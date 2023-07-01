#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

template <typename M_>
class CContest
{
public:
  //------------------------------------------------------------------------------------------------------------------
  CContest() // Default Constructor
  {}
  //------------------------------------------------------------------------------------------------------------------
  ~CContest() // destructor
  {}
  //------------------------------------------------------------------------------------------------------------------
  CContest& addMatch(string a, string b, M_ result) // addMatch
  {
    if(a == b) throw logic_error("Wrong!");
    if(m_Matches.count(make_pair(a,b)) != 0 || m_Matches.count(make_pair(b,a)) != 0)
      throw logic_error("Wrong!");
    m_Matches.insert({{a, b}, result});
    return *this;
  }
  //------------------------------------------------------------------------------------------------------------------
  bool isOrdered(const function<int(const M_& x)> cmp) const // CANNOT MODIFY
  {
    unordered_map<string,vector<string>> team_graph;
    for(auto it = m_Matches.begin(); it != m_Matches.end(); ++it) // load to the team_graph
    {
      if(cmp(it->second) > 0)
      {
        team_graph[it->first.first].push_back(it->first.second);
        team_graph[it->first.second];
      }
      else if(cmp(it->second) < 0)
      {
        team_graph[it->first.second].push_back(it->first.first);
        team_graph[it->first.first];
      }
    }

    unordered_map<string, int> team_connections; // number of connections of each team = rank of the team
    for (auto it = team_graph.begin(); it != team_graph.end(); it++) {
      unordered_set<string> visited;
      dfs(it->first, team_graph, visited);
      team_connections[it->first] = visited.size();
    }

    vector<pair<string,int>> final_ranking(team_connections.begin(), team_connections.end());
    sort(final_ranking.begin(), final_ranking.end(),
         [](pair<string, int>& a, pair<string, int>& b) { return a.second > b.second; });

    for(size_t i = 0; i < final_ranking.size(); i++) // check the same points in ranking
    {
      if(final_ranking[i].second == final_ranking[i+1].second)
        return false;
    }
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------
  list<string> results (const function<int(const M_& x)> cmp) const
  {
    list<string> final_list;
    if(isOrdered(cmp))
    {
      unordered_map<string,vector<string>> team_graph;
      for(auto it = m_Matches.begin(); it != m_Matches.end(); ++it) // load to the team_graph
      {
        if(cmp(it->second) > 0)
        {
          team_graph[it->first.first].push_back(it->first.second);
          team_graph[it->first.second];
        }
        else if(cmp(it->second) < 0)
        {
          team_graph[it->first.second].push_back(it->first.first);
          team_graph[it->first.first];
        }
      }

      unordered_map<string, int> team_connections; // number of connections of each team = rank of the team
      for (auto it = team_graph.begin(); it != team_graph.end(); it++) {
        unordered_set<string> visited;
        dfs(it->first, team_graph, visited);
        team_connections[it->first] = visited.size();
      }

      vector<pair<string,int>> final_ranking(team_connections.begin(), team_connections.end());
      sort(final_ranking.begin(), final_ranking.end(),
           [](pair<string, int>& a, pair<string, int>& b) { return a.second > b.second; });

      for(auto it = final_ranking.begin(); it != final_ranking.end(); it++)
        final_list.push_back(it->first);

      return final_list;
    }
    throw logic_error("Error!");
  }
  //--------------------------------------------------------------------------------------------------------------------
private:
  map<pair<string, string>, M_> m_Matches;
  //--------------------------------------------------------------------------------------------------------------------
  void dfs(string node, unordered_map<string, vector<string>>& graph, unordered_set<string>& visited) const
  {
    visited.insert(node);
    for(size_t i = 0; i < graph[node].size(); i++)
    {
      string neighbor = graph[node][i];
      if(visited.count(neighbor) == 0)
        dfs(neighbor, graph, visited);
    }
  }
  //--------------------------------------------------------------------------------------------------------------------
};

#ifndef __PROGTEST__
//======================================================================================================================
struct CMatch
{
public:
  CMatch(int a, int b)
    : m_A ( a ),
      m_B ( b )
  {}

  int m_A;
  int m_B;
};
//======================================================================================================================
class HigherScoreThreshold
{
public:
  HigherScoreThreshold(int diffAtLeast)
    : m_DiffAtLeast ( diffAtLeast )
  {}
  //------------------------------------------------------------------------------------------------------------------
  int operator ()(const CMatch& x) const
  {
    return (x . m_A > x . m_B + m_DiffAtLeast) - (x . m_B > x . m_A + m_DiffAtLeast);
  }
  //------------------------------------------------------------------------------------------------------------------
private:
  int m_DiffAtLeast;
};

int HigherScore(const CMatch& x)
{
  return (x . m_A > x . m_B) - (x . m_B > x . m_A);
}

int main(void)
{
  /*
  // MyTests 1.

  CContest<CMatch> a;
  a . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) )
    . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) );
  //a.addMatch("C++", "Pascal", CMatch ( 10, 3 ));
  //a.addMatch("Pascal", "C++", CMatch(1,0));
  a.isOrdered(HigherScore);
  a.results(HigherScore);
  a.isOrdered ( [] ( const CMatch & x ){return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A );} );
  a.results( [] ( const CMatch & x ){return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A );} );



  // MyTests 2.
  CContest<bool>  y;

  y . addMatch ( "Python", "PHP", true )
    . addMatch ( "PHP", "Perl", true )
    . addMatch ( "Perl", "Bash", true )
    . addMatch ( "Bash", "JavaScript", true )
    . addMatch ( "JavaScript", "VBScript", true );
  y . isOrdered ( [] ( bool v ){return v ? 10 : - 10;} );
  y . results ( [] ( bool v ){return v ? 10 : - 10;});


  */
  CContest<CMatch>  x;

  x . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );


  assert ( ! x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  x . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) );

  assert ( x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }

  assert ( ! x . isOrdered ( HigherScoreThreshold ( 3 ) ) );
  try
  {
    list<string> res = x . results ( HigherScoreThreshold ( 3 ) );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  assert ( x . isOrdered ( [] ( const CMatch & x )
  {
    return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A );
  } ) );
  try
  {
    list<string> res = x . results ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A );
    } );
    assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }

  CContest<bool>  y;

  y . addMatch ( "Python", "PHP", true )
    . addMatch ( "PHP", "Perl", true )
    . addMatch ( "Perl", "Bash", true )
    . addMatch ( "Bash", "JavaScript", true )
    . addMatch ( "JavaScript", "VBScript", true );

  assert ( y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    });
    assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }

  y . addMatch ( "PHP", "JavaScript", false );
  assert ( !y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  try
  {
    y . addMatch ( "PHP", "JavaScript", false );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  try
  {
    y . addMatch ( "JavaScript", "PHP", true );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
