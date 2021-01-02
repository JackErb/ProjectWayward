#include <logs.h>
#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

void cprint(string msg) {
  cout << msg << endl;
}

void fassert(bool condition, string msg) {
  if (condition) {
    return;
  }
  fatalerror(msg);
}

void fatalerror(string msg) {
  cerr << msg << endl;
  exit(1);
}
