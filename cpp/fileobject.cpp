#include "fileobject.h"

int csv::CSVReader::parseSaveField() {
  int res = 0;
  try {
    currfields.append(fieldbuf);
    fieldbuf.remove();
  } catch (std::bad_alloc& ba) {
    res = -1;
  }
  return res;
}

int csv::CSVReader::parseAddChar(Uchar32 uc) {
  int res = 0;
  try {
    fieldbuf += uc;
  } catch (std::exception& e) {
    res = -1;
  }
  return res;
}

int csv::CSVReader::parsing(Uchar32 uc) {}
