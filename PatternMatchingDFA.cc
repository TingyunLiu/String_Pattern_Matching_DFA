#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

bool isSuffixOf (string smallString, string fullString) {
  int full_len = fullString.size();
  int small_len = smallString.size();
  return (full_len >= small_len) &&
         (fullString.compare(full_len - small_len, small_len, smallString) == 0);
}

class PatternMatchingDFA{
private:
  string pattern;
  int len_of_pattern;
  vector<char> alphabets;
  vector<map<char,int> > trans_table;

public:
  // returns the integer m = the length of the pattern
  int length() {
    return len_of_pattern;
  }

  // returns the number of pairwise distinct characters in the pattern
  int numberOfCharacters() {
    return alphabets.size();
  }

  // constructor
  PatternMatchingDFA(string& s) : pattern(s), len_of_pattern(s.length()) {
    for (int i = 0; i < len_of_pattern; ++i) {
      if (find(alphabets.begin(), alphabets.end(), s[i]) != alphabets.end()) { // new char
        // already exist  do nothing
      } else {
        alphabets.push_back(pattern[i]);
      }
    }
  }


  // transition function to output the next state
  int transition(int q, char c) {   
    if (find(alphabets.begin(), alphabets.end(), c) != alphabets.end()) { // not in alphabets
      string temp = pattern.substr(0, q);
      string next(1,c);
      temp = temp + next;

      string sub = pattern;

      while (sub != "") {
        int len = sub.size();
        if (isSuffixOf(sub, temp)) {
          return len;
        }
        // drop off the last characters
        sub = sub.substr(0,len-1);
      }

      return 0; // go to state 0

    } else {
      return 0; // go to state 0
    }
  }

  // construct the transition table based on the values of transition function
  void build_trans_table() {
    ofstream out;
    out.open("a5delta.txt");

    for (int i = 0; i < len_of_pattern+1; ++i) {
      map<char,int> temp;
      for (int j = 0; j < alphabets.size(); ++j) {
        int next_state = transition(i,alphabets[j]);
        temp[ alphabets[j] ] = next_state;

        // output triples to file
        out << i << " " << alphabets[j] << " " << next_state << endl;
      }
      trans_table.push_back(temp);
    }

    out.close();
  }

  // find match
  void Matcher(string& text) {
    
    build_trans_table();

    ofstream out;
    out.open("a5matches.txt");

    int state = 0;
    for (int i = 0; i < text.length(); ++i) {
      map<char,int> temp = trans_table[state];
      state = temp[text[i]];
      if (state == len_of_pattern) {
        out << i - (len_of_pattern-1) << endl;
      }
    }

    out.close();
  }

};


int main(int argc, char **argv){

  string pattern, text, temp;
  
  // read pattern
  ifstream in;
  in.open("a5pattern.txt");
  in >> pattern;
  PatternMatchingDFA dfa(pattern);
  in.close();

  // read text
  in.open("a5text.txt");
  while(!in.eof()) {
    in >> temp;
    text = text + temp;
  }
  in.close();

  dfa.Matcher(text);
  return 0;
}

