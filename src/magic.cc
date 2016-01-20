//g++ -L/usr/lib64 -lmagic magic.cpp -o magic
#include <stdio.h>
#include <magic.h>
#include <string>
#include <iostream>
#include "global.hpp"
#include "log.hpp"

using namespace::std;
int string_regex(std::string str, std::string pattern);

string file_magic(std::string filename ){
  
  magic_t myt = magic_open(MAGIC_MIME_ENCODING);
  magic_load(myt,NULL);
  std::string magic_full = magic_file(myt, filename.c_str());
  magic_close(myt);
  std::string file_type = magic_full;
  return file_type;
}
