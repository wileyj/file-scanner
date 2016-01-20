//epoch.cpp 

#include <time.h>
#include <unistd.h>
#include <string>
#include "global.hpp"
#include "log.hpp"

using std::string;

string returnDate(int epoch){
  time_t t = (time_t) epoch; 
  string date = asctime(localtime(&t));
  date.erase (date.length()-1);
  return date;
}
string returnTime(){
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime (buffer,80,"[%b %d - %H:%M:%S]",timeinfo);
  return buffer;
}
string formattedTime(){
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime (buffer,80,".%Y%m%d_%H%M%S",timeinfo);
  return buffer;
}
