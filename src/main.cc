#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include "global.hpp"
#include "log.hpp"

using namespace::std;

std::string path;
bool debug     = false;
bool email     = false; 
bool silent    = false;
bool c_symlink = false;
bool q_file    = false;
bool progress  = false;
bool show_all  = false;
bool with_sha1 = false;
bool help      = false;
int vlevel     = 0;
char output_log[16] = "output_log.txt";
ofstream outputfile;
std::string sl= "/";
std::string cwd = getcwd(NULL, 0);
typedef std::vector <std::string> DirListing_t;
const char* dot;
int totalfiles;
int matchfiles;
std::vector<std::string> v_include;
std::vector<std::string> v_backdoors;
std::vector<std::string> v_exclude;
std::vector<std::string> quarantined;
std::vector<std::string> f_output;
std::vector<std::string> s_output;
std::string perm_s = "0000";
int perm = 000;
char mode[] = "0000";
const char* user_id;
const char* group_id;
std::string a_time;
std::string c_time;
std::string m_time;

int GetDirListing( DirListing_t& result, const std::string& dirpath );
int showHelp( char binary[] );
void read_file( char *filename );
int cleanlogs(char logfile[16]);
int movelogs(char logfile[16]);
void logger(std::string output, char output_log[16]);
void send_email(char output_log[16]);
int send_mail(const char *smtpserver, const char *from, const char *to, const char *subject, const char *replyto, const char *msg);
bool f_read(const char *filename);

int main( int argc, char* argv[] ){
  cout<<std::endl;
  int c;
  while ( (c = getopt(argc, argv, "veslqpaw")) != -1) {
    //int this_option_optind = optind ? optind : 1;
    switch (c) {
      case 'v':
        vlevel++;
        break;
      case 'e':
	email = true;
        break;
      case 'q':
        q_file = true;
        break;
      case 'p':
        progress = true;
        break;
      case 'a':
        show_all = true;
        break;
      case 'w':
        with_sha1 = true;
        break;
      case 's':
        silent = true;
        break;
      case 'l':
        c_symlink = true;
        break;
      case '?':
        showHelp(argv[0]);
        return 0;
        break;
      default:
        showHelp(argv[0]);
        break;
    }
  }
  int diff = argc - optind;
  if(vlevel > 4){
    vlevel = 4;
  }
  if (diff == 1) {
    LOG(vlevel,cout,"Found non-option ARGV-elements:"<<std::endl);
    string g_char = argv[optind++];
    const char* thisPath = g_char.c_str();
    if (strcmp(thisPath,"") == 0){
      LOG(vlevel,cout, "Path Var is NULL");
      LOG(vlevel,cout, "Error - No valid directory path found on cmdline");
      showHelp(argv[0]);
      return 0;
    }
    // remove old logfiles first
    if(cleanlogs(output_log) != 0){
      LOG(vlevel,cout, "Old Logs ("<<output_log<<") not removed...");
    }
    if(f_read(output_log) == 1){
      if(movelogs(output_log) != 0){
        LOG(vlevel,cout, "File ("<<output_log<<") file not rolled...");
      }
    }
    if(f_read(output_log) == 1){
      if( remove(output_log) != 0 ){
        LOG(vlevel,cout,"Error deleting file: "<<output_log);
      }else{
        LOG(vlevel,cout,"File "<<output_log<<" successfully deleted");
      }
    }


    struct stat sb;
    if (stat(thisPath, &sb) == 0 && S_ISDIR(sb.st_mode)){
      path = thisPath;
    }
  }else{
    while (optind < argc){
      LOG(vlevel,cout,"Unknown Vars:"<<argv[optind++]<<std::endl);
    }
    showHelp(argv[0]);
    return 0;
  }
  dot = ".";
  if (vlevel > 0 && silent == 1){
    LOG(vlevel,cout,"Verbosity enabled with silent mode. Verbosity overrides silent"<<std::endl);
  }
  LOG(vlevel,cout, "Verbosity Level:    "<<vlevel);
  LOG(vlevel,cout, "Debug enabled:      "<<debug);
  LOG(vlevel,cout, "Silent enabled:     "<<silent);
  LOG(vlevel,cout, "Quarantine Enabled: "<<q_file);
  LOG(vlevel,cout, "Symlink enabled:    "<<c_symlink);
  LOG(vlevel,cout, "Progress enabled:   "<<progress);
  LOG(vlevel,cout, "Show All Details:   "<<show_all);
  LOG(vlevel,cout, "With Sha1sum:       "<<with_sha1);
  if (path.compare(dot)==0){
    LOG(vlevel,cout, "Path is a dot" <<path);
    path = cwd;
  }
  LOG(vlevel,cout, "Path is currently set to: " << path);
  LOG(vlevel, cout, "Reading file: "<<include);
  read_file(include);
  read_file(exclude);
  read_file(backdoors);
  int size_v = v_include.size();
  int size_e = v_exclude.size();
  int size_b = v_backdoors.size();
  if (silent == 0){
    cout<<"Number of includes  to match : "<<size_v<<std::endl;
    cout<<"Number of excludes  to ignore: "<<size_e<<std::endl;
    cout<<"Number of backdoors to match : "<<size_b<<std::endl;
    cout<<"-------------------------------------------------------------"<<std::endl<<std::endl;
  }
  DirListing_t dirtree;
  GetDirListing( dirtree, path);
  int size_q = quarantined.size();
  stringstream stream_q;
  stream_q << size_q;
  string q_size = stream_q.str();
  
  int size_f = f_output.size();
  stringstream stream_f;
  stream_f << size_f;
  string f_size = stream_f.str();

  if (c_symlink == 0){
    LOG(vlevel,cout,"Found "<<f_size<<" matching records:");
    LOG(vlevel,cout,"Found "<<q_size<<" quarantined records:");
    if (show_all == 1){cout<<std::endl;}
    std::string match_f = "Found " + f_size + " Matching files:";
    logger(match_f, output_log);
    for(int i=0;i<size_f;i++){
      logger(f_output[i], output_log);
    }
    logger("-------------------------------------------------------------",output_log);
    logger("", output_log);
    if (q_file == 1 ){
      std::string match_q = "Quarantined " + q_size + " files:";
      logger(match_q, output_log);
      for(int i=0;i<size_q;i++){
        logger(quarantined[i], output_log);
      }
      logger("-------------------------------------------------------------",output_log);
      logger("", output_log);
    }else{
      std::string match_q = "Files Not Quarantined (no \"-q\" flag set):";
      logger(match_q, output_log);
      for(int i=0;i<size_q;i++){
        logger(quarantined[i], output_log);
      }
      logger("-------------------------------------------------------------",output_log);
      logger("", output_log);
    }
  }else{
    int size_c = s_output.size();
    stringstream stream_c;
    stream_c << size_c;
    string c_size = stream_c.str();
    LOG(vlevel, cout, "Found "<<c_size<<" matching symlinks:");
    std::string match_c = "Found: " + c_size + " Symlinks";
    logger(match_c, output_log);
    for(int i=0;i<size_c;i++){
      logger(s_output[i], output_log);
    }
    logger("-------------------------------------------------------------",output_log);
    logger("", output_log);
  }
  stringstream match_f;
  match_f << matchfiles;
  string m_size = match_f.str();
  std::string matchfiles_m = "Total Files Matched: " + m_size;
  logger(matchfiles_m, output_log);

  stringstream total_f;
  total_f << totalfiles;
  string t_size = total_f.str();
  std::string totalfiles_m = "Total Files Read:    " + t_size;
  logger(totalfiles_m, output_log);

  if (silent == 0){
    cout<<std::endl<<std::endl;
  }
  if (email == 1){
    LOG(vlevel,cout,"Email option was enabled. Sending Email...");
    send_email(output_log);
  }
  return 0;

}
