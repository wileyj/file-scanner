//help.cpp 
#include <iostream>

using namespace std;

int showHelp( char binary[] ){
  cout << "usage:\n  " <<  binary << "<dirpath>\n"
  << "\t-v - Enable verbose logging (-vvvv)\n"
  << "\t-e - Send email of output\n"
  << "\t-s - Suppress all output\n"
  << "\t-w - With sha1sum\n"
  << "\t-l - Only show symlinks\n"
  << "\t-a - Show All files as they are checked\n"
  << "\t-p - Show Progress in number of files read\n"
  << "\t-q - Quarantine files (chmod 000)\n";
  return 0;
}
