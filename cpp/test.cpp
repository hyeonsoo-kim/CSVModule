#include "fileobject.h"
#include "uchario.h"

int main() {
#ifdef ARRLIST_TEST
  // ArrayList Test

  ArrayList<int> IntList;

  std::cout << "[*] IntList = " << IntList
            << "\n[+] 1부터 100까지 IntList에 저장하기" << std::endl;
  for (int i = 1; i <= 100; i++) {
    IntList.append(i);
  }

  std::cout << "[*] 저장 완료. IntList = " << IntList << std::endl;

  ArrayList<int> IntList2(IntList);
  std::cout << std::boolalpha
            << "[*] IntList == IntList2 : " << (IntList == IntList2)
            << std::endl;
#else
#ifdef CSV_TEST
  csv::CSVReader x("yallacha.csv");
  std::cout << x.getdata() << std::endl;
#endif
#endif
  return 0;
}