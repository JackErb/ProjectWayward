#include <logs.h>
#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

void cprintf(string msg) {
  cout << msg << endl;
}

void fassert(bool condition, string msg) {
  if (!condition) {
    fatalerror(msg);
  }
}

void fatalerror(string msg) {
  cerr << msg << endl;
  exit(1);
}
