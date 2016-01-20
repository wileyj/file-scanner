#include <iostream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <sstream>
#include "global.hpp"
#include "log.hpp"

using namespace::std;
struct stat64 st;

string returnDate(int epoch);
void checkSymlink(char filename [ ], string type){
  char buf[512];
  int count = readlink(filename, buf, sizeof(buf));
  if(count > 0){
    buf[count] = '\0';
    stat64(buf, &st);
    int mtime   = st.st_mtime;
    struct passwd *pw     = getpwuid(st.st_uid);
    struct group  *gr     = getgrgid(st.st_gid);
    std::ostringstream ss_o;
    std::string s_o;
    if (pw != 0){
      user_id = pw->pw_name;
    }else{
      ss_o << st.st_uid;
      s_o = ss_o.str();
      user_id = s_o.c_str();
    }
    if (gr != 0){
      group_id = gr->gr_name;
    }else{
      ss_o << st.st_uid;
      s_o = ss_o.str();
      group_id = s_o.c_str();
    }
    std::string s_time = returnDate(mtime);
    std::string final_output = type + " " + s_time + " " + user_id + ":" + group_id + " " + filename + " -> " + buf;
    LOG(vlevel, cout, "Link: "<<final_output);
    s_output.push_back(final_output); 
  }
}
