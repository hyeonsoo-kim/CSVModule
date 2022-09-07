

#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/umachine.h>
#include <unicode/uobject.h>
#include <unicode/utf16.h>
#include <unicode/utypes.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

#include "arraylist.hpp"
#include "uchario.h"

#define TRUE 1
#define FALSE 0

using namespace std;
/*
/* Referenced to icu/io/ustream.cpp
 (https://github.com/unicode-org/icu/blob/main/icu4c/source/io/ustream.cpp)
istream& operator>>(istream& is, UChar& x) {
  if (is.fail()) {
    return is;
  }

  char buffer[16];
  int32_t idx = 0;

  UChar Ubuffer[1];

  UConverter* converter;
  UErrorCode errcode = U_ZERO_ERROR;

  converter = ucnv_open(NULL, &errcode);

  if (U_SUCCESS(errcode)) {
    UChar* us = Ubuffer;
    const UChar* uLimit =
        Ubuffer + static_cast<int32_t>((sizeof(Ubuffer) / sizeof(Ubuffer[0])));
    const char *s, *sLimit;
    char ch;
    UChar ch32;
    UBool initialWhitespace = TRUE;
    UBool continueReading = TRUE;

    while (continueReading) {
      ch = is.get();
      if (is.eof()) {
        if (!initialWhitespace) {
          is.clear(is.eofbit);
        }
        continueReading = FALSE;
      }

      sLimit = &ch + 1;
      us = Ubuffer;
      s = &ch;
      errcode = U_ZERO_ERROR;

      ucnv_toUnicode(converter, &us, uLimit, &s, sLimit, 0, !continueReading,
                     &errcode);
      if (U_FAILURE(errcode)) {
        is.clear(is.failbit);
        goto STOP;
      }

      if (us != Ubuffer) {
        int32_t ubsize = static_cast<int32_t>(us - Ubuffer);
        int32_t ubidx = 0;
        while (ubidx < ubsize) {
          U16_NEXT(Ubuffer, ubidx, ubsize, ch32);
          if (u_isWhitespace(ch32)) {
            if (!initialWhitespace) {
              buffer[idx++] = ch;
              while (idx > 0) {
                is.putback(buffer[--idx]);
              }
              goto STOP;
            }
          } else {
            if (initialWhitespace) {
              initialWhitespace = FALSE;
            }
            x = ch32;
            goto STOP;
          }
        }
        idx = 0;

      } else {
        buffer[idx++] = ch;
      }
    }
  STOP:
    ucnv_close(converter);
  }
  return is;
}

ostream& operator<<(ostream& os, UChar& x) { return os; }*/
#include <unicode/unistr.h>
#include <unicode/ustream.h>

int main() {
  // UFILE* file = u_fopen("yallacha.csv", "r", NULL, NULL);
  // UChar x = u_fgetc(file);

  ifstream file("yallacha.csv");
  UChar x, y;
  cin >> x;
  cout << icu::UnicodeString(x) << endl;
  file.close();
  return 0;
}