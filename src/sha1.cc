//sha1.cpp
//g++ sha1.c++  -lssl

#include <openssl/sha.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sstream>
#include "global.hpp"
#include "log.hpp"

#define MAX_BUF_LEN  (128)

using namespace std;

//string getSHA1(char *filename){
string getSHA1(std::string file){
  FILE *fd;
  unsigned char buf[MAX_BUF_LEN];
  int i=0;
  SHA_CTX sc;
  unsigned char hash[SHA_DIGEST_LENGTH];
  const char *filename = file.c_str();
  fd=fopen(filename,"rb");
  SHA1_Init(&sc);
  for (;;) {
    size_t len;
    len = fread(buf, 1, sizeof buf, fd);
    if (len == 0)
      break;
      SHA1_Update(&sc, buf, len);
  }
  SHA1_Final(hash,&sc);
  char out[SHA_DIGEST_LENGTH*2];
  while ( i<SHA_DIGEST_LENGTH) {
    sprintf((char*)&(out[i*2]), "%02x", (unsigned int)hash[i]);
    i++;
  }
  fclose(fd);

  stringstream ss;
  std::string s;
  ss << out;
  ss >> s;

  return s;
}
