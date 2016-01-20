//filestats.cpp

#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <string>
#include <magic.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include "global.hpp"
#include "log.hpp"

#define MODE(z) (( st.st_mode & (z)) == (z))

using namespace std;
string returnDate(int epoch);
void fileStats( std::string file ){
  const char *filename  = file.c_str();
  struct stat64 st;
  stat64(filename, &st);
  perm = 000;
  int atime   = st.st_atime;
  int ctime   = st.st_ctime;
  int mtime   = st.st_mtime;
  //int mode    = st.st_mode;

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
  char suid = (MODE(S_ISUID)? '4' : '0');
  int perm_setuid = int(suid)-48;
  char perm_ur = (MODE(S_IRUSR)? '4' : '0');
  char perm_uw = (MODE(S_IWUSR)? '2' : '0');
  char perm_ux = (MODE(S_IXUSR)? '1' : '0');
    int perm_ur_i = int(perm_ur)-48;
    int perm_uw_i = int(perm_uw)-48;
    int perm_ux_i = int(perm_ux)-48;
    int user_p = perm_ur_i + perm_uw_i + perm_ux_i;
  char perm_gr = (MODE(S_IRGRP)? '4' : '0');
  char perm_gw = (MODE(S_IWGRP)? '2' : '0');
  char perm_gx = (MODE(S_IXGRP)? '1' : '0');
    int perm_gr_i = int(perm_gr)-48;
    int perm_gw_i = int(perm_gw)-48;
    int perm_gx_i = int(perm_gx)-48;
    int group_p = perm_gr_i + perm_gw_i + perm_gx_i;
  char perm_or = (MODE(S_IROTH)? '4' : '0');
  char perm_ow = (MODE(S_IWOTH)? '2' : '0');
  char perm_ox = (MODE(S_IXOTH)? '1' : '0');
    int perm_or_i = int(perm_or)-48;
    int perm_ow_i = int(perm_ow)-48;
    int perm_ox_i = int(perm_ox)-48;
    int other_p = perm_or_i + perm_ow_i + perm_ox_i;
  perm = (perm_setuid*1000)+(user_p*100)+(group_p*10)+other_p;

  a_time = returnDate(atime);
  c_time = returnDate(ctime);
  m_time = returnDate(mtime);
  if (perm == 0){
    perm_s = "0000";
  }else{
    stringstream perm_ss;
    perm_ss << perm;
    perm_s = perm_ss.str();
    if (perm < 1000){
      perm_s = "0" + perm_s;
    }
  }
}

/*
mode_t st_mode  - Specifies the mode of the file. This includes file type information and the file permission bits (see Permission Bits). 
uid_t st_uid    - The user ID of the file's owner. See File Owner. 
gid_t st_gid    - The group ID of the file. See File Owner. 
off64_t st_size - This specifies the size of a regular file in bytes. 
time_t st_atime - This is the last access time for the file. See File Times. 
time_t st_mtime - This is the time of the last modification to the contents of the file. See File Times. 
time_t st_ctime - This is the time of the last modification to the attributes of the file. See File Times. 
unsigned int st_blksize  - The optimal block size for reading of writing this file, in bytes. You might use this size for allocating the buffer space for reading of writing the file. (This is unrelated to st_blocks.)

http://stackoverflow.com/questions/1393564/get-owners-access-permissions-using-c-and-stat
http://linux.die.net/man/2/stat
*/
