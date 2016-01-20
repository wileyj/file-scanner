//log.cpp

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <string>
#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include "global.hpp"
#include "log.hpp"

using namespace std;
string formattedTime();
bool f_read(const char *filename);


void logger(std::string output, char output_log[16]){
  LOG(vlevel, cout, "LOGGER output_log: "<<output_log);
  outputfile.open(output_log, ios::out | ios::app);
  outputfile<<output<<std::endl;
  outputfile.close();
  if (silent == 0){
    cout<<output<<std::endl;
  }
}

int movelogs(char logfile[16]){
  ofstream outputfile;
  std::string output_f;
  std::stringstream sl;
  sl << logfile;
  sl >> output_f;
  if (f_read(logfile) == 1){
    std::string out_f_append = output_f.append(formattedTime());
    char *out_f_archive = (char*)out_f_append.c_str();
    if(f_read(logfile) == 1){
      if(rename( logfile, out_f_archive) == 0){
        LOG(vlevel,cout,"Moved: "<<logfile<<" to -> "<<out_f_archive<<std::endl);
      }else{
        LOG(vlevel,cout,"Error renaming file: "<<logfile<<std::endl);
        return 1;
      }
    }
  }
  return 0;
}
int cleanlogs(char logfile[16]){
  int char_len;
  if(strcmp(logfile, "process_log.txt") == 0){
    char_len = 15;
  }else if(strcmp(logfile, "output_log.txt") == 0){
    char_len = 14;
  }else{
    char_len = 11;
  }
  vector<string> files;
  DIR *dir;
  struct dirent *ent;
  dir = opendir (".");
  int i=0;
  if (dir != NULL) {
    while ((ent = readdir (dir)) != NULL){
      //if(strncmp("output.txt.",ent->d_name,11) == 0){
      if(strncmp(logfile,ent->d_name,char_len) == 0){
        files.push_back(ent->d_name);
        i++;
      }
    }
    closedir (dir);
  }
  int len = files.size();
  sort (files.begin(), files.end());
  reverse (files.begin(), files.end());
  for(i=5;i<len;i++){
    LOG(vlevel,cout,"Removing Expired file: "<<files[i]);
    if(f_read(files[i].c_str()) == 1){
      if( remove(files[i].c_str()) != 0 ){
        LOG(vlevel,cout,"Error Deleting Expired file: "<<files[i]);
      }
    }
  }
  return 0;
}

