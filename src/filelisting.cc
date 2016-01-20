#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <sys/mount.h>
#include "global.hpp"
#include "log.hpp"

using namespace std;
typedef std::vector <std::string> DirListing_t;
typedef std::pair<std::string, std::string> str_pair;
typedef std::vector<str_pair> str_map;
typedef str_map::iterator map_iter;
typedef str_map::const_iterator map_citer;
std::string keyword;
string file_magic(std::string filename );
int string_regex(std::string str, std::string pattern);
void logger(std::string output, char output_log[16]);
string getSHA1(std::string filename);
void fileStats( std::string file);
int grepBackdoor( std::string file );
void checkSymlink(char filename [ ], string type);

std::string map_copy(const str_map& m, const std::string& str){
  std::string out;
  map_citer i;
  for(size_t pos = 0; pos < str.size(); ++pos){
    for(i = m.begin(); i != m.end() && str.find(i->first, pos) != pos; ++i);
      if(i != m.end()){
        out.append(i->second);
        pos += i->first.length() - 1;
        continue;
      }
    out.append(1, str[pos]);
  }
  return out;
}

int GetDirListing( DirListing_t& result, const std::string& dirpath ){
  int size_v = v_include.size();
  int size_e = v_exclude.size();
  int excluded = 0;
  DIR* dir = opendir( dirpath.c_str() );
  if (dir){
    struct dirent* entry;
    while ((entry = readdir( dir ))){
      struct stat entryinfo;
      std::string entryname = entry->d_name;
      std::string entrypath = dirpath + "/" + entryname;
      if (!stat( entrypath.c_str(), &entryinfo )){
        if (S_ISDIR( entryinfo.st_mode )){
          if      (entryname == ".."){}
          else if (entryname == "." ){
            result.push_back( dirpath + "/" );
          }else{
            if (c_symlink == 1){
              LOG(vlevel,cout,"Found symlink: "<<entrypath);
              int size_d = entrypath.size();
              char *file_d = new char[size_d+1];
              memcpy(file_d, entrypath.c_str(), size_d+1);
              checkSymlink(file_d, "DIR");
            }
            GetDirListing( result, entrypath );
          }
        }else{
          totalfiles++;
          std::string full_path;
          std::string cwd = getcwd(NULL, 0);
          if (entrypath[0] == '.'){
            std::string full_path_a = cwd + "/" + entrypath;
            str_map map1;
            map1.push_back(str_pair("/./","/"));
            full_path = map_copy(map1, full_path_a);
          }else if(entrypath[0] == '/'){
            full_path = entrypath;
          }else{
            full_path = cwd + "/" + entrypath;
          }
          for (int j=0; j<size_e; j++){
            if (string_regex(full_path, v_exclude[j]) == 1){
              LOG(vlevel,cout,"Excluding: "<<full_path);
              excluded = 1;
            }
          } 
          if (excluded == 0){
            LOG(vlevel,cout,"Checking file: "<<full_path);
            if (c_symlink == 1){
              int size_f = full_path.size();
              char *file_f = new char[size_f+1];
              memcpy(file_f, full_path.c_str(), size_f+1);
              checkSymlink(file_f, "FILE");
            }else{
	      fileStats(full_path);
	      std::string returned = file_magic(full_path );
              for (int i=0; i<size_v; i++){
                if(string_regex(returned,v_include[i]) == 1){
                  std::string sha1;
                  if(with_sha1 == 1){
                    sha1 = getSHA1(full_path);
                  }else{
                    sha1 = "";
                  }
                  if (grepBackdoor(full_path) == 1){
                    if (q_file == 1){
                      std::string final_output;
                      if (strcmp(perm_s.c_str(), "0000")!=0){
                        //LOG(vlevel, cout, "Removing all perms for: "<<full_path);
                        cout<<"Removing all perms for: "<<full_path<<std::endl;
	                final_output = full_path + " -> Matched: \"" + keyword + "\"";
                        chmod(full_path.c_str(),0000);
                      }else{
                        //LOG(vlevel,cout,"File: "<<full_path<<" Already Quarantined with permissions: "<<perm_s);
                        cout<<"File: "<<full_path<<" Already Quarantined with permissions: "<<perm_s<<std::endl;
                        final_output = full_path + " -> Matched: \"" + keyword + "\" - Currently Quarantined";
                      }
		      quarantined.push_back(final_output);
                    }else{
                      std::string n_q;
                      if (strcmp(perm_s.c_str(), "0000")!=0){
                        n_q = "File: " + full_path + " Was NOT Quarantined (matched:" + keyword + ")";
                      }else{
                        n_q = "File: " + full_path + " Already Quarantined with permissions: " + perm_s;
                      }
                      quarantined.push_back(n_q);
                    }
                  }
                  std::string final_output = c_time + " " + perm_s +  " " + sha1 + " \"" + returned + "\" "  + full_path;
                  LOG(vlevel,cout,"OUT: "<<final_output);
                  if (silent == 0 && show_all == 1){
                    cout<<final_output<<std::endl;
                  }
	          f_output.push_back(final_output);
                  matchfiles++;
                  if (progress == 1){
                    fflush(stdout);
                    printf("Files Read: %d\r", matchfiles);
                  }
                  result.push_back( full_path );
                }
              }
            }
          }
        }
      }
    }
    closedir( dir );
  }
  return matchfiles;  
}


int grepBackdoor( std::string file ){
  keyword="";
  int found = 0;
  LOG(vlevel, cout,"Looking for backdoor string in file: "<<file);
  std::string line;
  ifstream in(file.c_str());
  if( in.is_open()){
    while( getline(in,line) ){
      for(size_t i = 0; i < v_backdoors.size(); ++i){
        if(string_regex( line.c_str() , v_backdoors[i]) == 1){
          keyword = v_backdoors[i];
          LOG(vlevel, cout, "FOUND a backdoor in file ("<<keyword<<"): "<<file);
          LOG(vlevel, cout, "line: "<<line.c_str());
	  i = v_backdoors.size();
          found = 1;
        }
      }
    }
  }
  return found;
}


