#include "fileobject.h"
#include "uchario.h"

int main() {
  csv::CSVReader x("yallacha.csv");
  std::cout << x.getdata() << std::endl;
  return 0;
}