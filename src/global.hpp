//global.h

#ifndef _GLOBAL_H_ 
#define _GLOBAL_H_
#include <vector>
#include <iostream>
#include <map>
 
#define include      "../etc/include"
#define backdoors    "../etc/backdoors.txt"
#define exclude      "../etc/exclude"
#define email_config "../etc/scan.config"
#define db_server    "localhost";
#define db_user      "scanner";
#define db_password  "scanner";
#define db_database  "file_scanner";

typedef std::vector <std::string> DirListing_t;
extern bool email; 
extern bool debug;
extern bool silent;
extern bool c_symlink;
extern bool help;
extern bool q_file;
extern bool progress;
extern bool show_all;
extern bool with_sha1;
extern int vlevel;
extern char output_log[16];
extern int numfiles;
extern int totalfiles;
extern int matchfiles;

extern std::vector<std::string> v_include;
extern std::vector<std::string> v_backdoors;
extern std::vector<std::string> v_exclude;
extern std::vector<std::string> quarantined;
extern std::vector<std::string> f_output;
extern std::vector<std::string> s_output;

extern const char* dot;
extern std::string cwd;
extern std::ofstream outputfile;
extern std::string perm_s;
extern int perm;
extern char mode[];
extern const char* user_id;
extern const char* group_id;
extern std::string a_time;
extern std::string c_time;
extern std::string m_time;

#endif
