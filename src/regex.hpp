#ifndef _REGEX_H_ 
#define _REGEX_H_

#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>

template <class _II, class _OI>
static _OI escapeSomeChars(const _II inIt, const _II endIt, _OI outIt){
  for (_II it=inIt; it!=endIt; ++it){
    switch (*it){
      case '\0': outIt++ = '\\'; outIt++ = '0'; break;
      case '\n': outIt++ = '\\'; outIt++ = 'n'; break;
      case '(' : 
      case '\\':
      case '"' :
      case '$' :
      case '/' : outIt++ = '\\';
      default  : outIt++ = *it;
    }
  }
  return outIt;
}
static std::string escapeSomeChars(const std::string& input){
  std::ostringstream os;
  escapeSomeChars(input.begin(), input.end(), std::ostream_iterator<char>(os));
  return os.str();
}

namespace {
  struct rawchar {
    char _c; rawchar(char c=0) : _c(c) {}
    operator const char&() const { return _c; }
    friend std::istream& operator>>(std::istream& is, rawchar& out) { return is.get(out._c); }
  };
}

//static std::string delUnnecessary(string &str){
static std::string delUnnecessary( std::string& str){
  int size = str.length();
  for(int j = 0; j<=size; j++){
    for(int i = 0; i <=j; i++){
      if(str[i] == ' ' && str[i+1] == ' ' || str[i] == '*'){
        str.erase(str.begin() + i);
      }else if(str[0]== ' '){
        str.erase(str.begin());
      }else if(str[i] == '\0' && str[i-1]== ' '){
        str.erase(str.end() - 1);
      }
      str[i] = std::tolower(str[i]);
    }
  }
  return str;
}

#endif
