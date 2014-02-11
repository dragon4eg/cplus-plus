#include "Reporter.h"
#include <iostream>
using std::cout;
using std::endl;


int main()
{
  Reporter r;
  Reporter::report();
  cout << endl;

  Reporter rcopy(r);
  Reporter::report();
  cout << endl;

  {
    Reporter rscoped;
    Reporter rscopedcopy(rscoped);
    Reporter::report();
    cout << endl;
  }

  Reporter::report();
  Reporter::report(false);

  return 0;
}

