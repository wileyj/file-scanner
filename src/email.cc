//email.cpp
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include "split.hpp"
#include "log.hpp"
#include "global.hpp"

using namespace std;
bool f_read(const char *filename);
void logger(std::string output, char output_log[16]);
int send_mail(const char *smtpserver, const char *from, const char *to, const char *subject, const char *replyto, const char *msg);

static std::string delUnnecessary( std::string& str, std::string& key){
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
      if(key != "subject"){
        str[i] = std::tolower(str[i]);
      }
    }
  }
  return str;
}
void send_email(char file[16]){
std::map<std::string, std::string> options;

  ifstream inputfile;
  LOG(vlevel,cout,"Reading file: "<<email_config);
  inputfile.open(email_config, ios::in);
  if(f_read(email_config) == 1){
    string myStr;
    while(!inputfile.eof()){
      string line;
      getline(inputfile,line);
      if(!line.empty()){
        Splitter split ( line, "=");
        std::string key = delUnnecessary(split[0],split[0]);
        std::string val = delUnnecessary(split[1],key);
        options[key] = val;
      }
    }
  }
  inputfile.close();

cout<<"options[smtp_server]:"<<options["smtp_server"]<<std::endl;
cout<<"options[subject]:"<<options["subject"]<<std::endl;
cout<<"options[from]:"<<options["from"]<<std::endl;
cout<<"options[replyto]:"<<options["replyto"]<<std::endl;
cout<<"options[rcpt]:"<<options["rcpt"]<<std::endl;


const char* mail_server = options["smtp_server"].c_str();
const char* mail_subject = options["subject"].c_str();
const char* mail_from = options["from"].c_str();
const char* mail_replyto = options["replyto"].c_str();
const char* mail_to = options["rcpt"].c_str();

  ofstream outputfile;
  std::ifstream read_output;
  int f_length;
  read_output.open(file);
  read_output.seekg(0, std::ios::end);
  f_length = read_output.tellg();
  read_output.seekg(0, std::ios::beg);
  char* buffer = new char[f_length];
  read_output.read(buffer, f_length);
  read_output.close();

  LOG(vlevel,cout,"SMTP Server: "<< mail_server);
  LOG(vlevel,cout,"Subject:     "<< mail_subject);
  LOG(vlevel,cout,"From:        "<< mail_from); 
  LOG(vlevel,cout,"Reply To:    "<< mail_replyto);
  LOG(vlevel,cout,"Mail To:     "<< mail_to);
  if (send_mail(mail_server, mail_from, mail_to, mail_subject, mail_replyto, buffer ) != 0){
    LOG(vlevel,cout,"Message send failed!");
  }else{
    LOG(vlevel,cout,"Message sent successfully!");
  }
}
