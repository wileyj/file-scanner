#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include "split.hpp"
#include "global.hpp"
#include "log.hpp"
#include "regex.hpp"


int string_regex(std::string str, std::string pattern);
using namespace::std;

string delUnnecessary(string &str);
template <class _II, class _OI>
static _OI escapeSomeChars(const _II inIt, const _II endIt, _OI outIt);
static std::string escapeSomeChars(const std::string& input);

bool f_read(const char *filename){
  ifstream ifile(filename);
  return ifile;
}
void read_file( char *filename ){
  ifstream inputfile;
  LOG(vlevel,cout,"Reading file: "<<filename);
  inputfile.open(filename, ios::in);
  if(f_read(filename) == 1){
    string myStr;
    while(!inputfile.eof()){
      string line;
      getline(inputfile,line);
      if(!line.empty()){
        stringstream st;
        std::string ty;
        st << line[0];
        st >> ty;
        if(ty.compare("#") == 1){
          if (strcmp(filename, include)==0){
            v_include.push_back(line);
 	    LOG(vlevel,cout,"Adding to includes: "<<line);
          }else if(strcmp(filename, exclude)==0){
            std::string str = delUnnecessary(line);
            v_exclude.push_back(str);
 	    LOG(vlevel,cout,"Adding to excludes: "<<line);
          }else if (strcmp(filename, backdoors)==0){
            std::string escaped = escapeSomeChars(line);
            v_backdoors.push_back(escaped);
 	    LOG(vlevel,cout,"Adding to backdoors: "<<escaped);
          }
        }
      }
    }
    inputfile.close();
    LOG(vlevel,cout,"-------------------------------------------------------------"<<std::endl);
  }else{
    cout<<"Error Reading file: "<<filename<<std::endl;
  }
}

