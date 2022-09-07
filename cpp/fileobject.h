#ifndef __CSVMODULE_FILEOBJECT_H__
#define __CSVMODULE_FILEOBJECT_H__

#include <unicode/umachine.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <new>
#include <stdexcept>
#include <string>

#include "arraylist.hpp"

namespace csv {

enum ParserState {
  START_RECORD = 0,  // Start of Recoed(Row)
  START_FIELD,       // Start of Field
  ESCAPED_CHAR,      // Possible escaped charater (escaped character may exsit)
  IN_FIELD,          // in unquoted field ( begin new unquoted field )
  IN_QUOTED_FIELD,   // in quoted field
  ESCAPE_IN_QUOTED_FIELD,  // Possible escaped character in quoted field
  QUOTE_IN_QUOTED_FIELD,   // doublequoted ( quote in quoted field)
  MATCH_NEWLINE,           // match newline
  AFTER_ESCAPED_NEWLINE    // after parse(escape) newline char
};

class CSVReader {
 public:
 private:
  std::ifstream file;                              // input filestream
  ArrayList<ArrayList<icu::UnicodeString> > data;  // List of Datas
  ParserState state;                               // current CSV parse state
  ArrayList<icu::Unicodestring> currfields;        // fields in current line
  icu::UnicodeString fieldbuf;                     // temporary buffer
  std::size_t line_num;                            // source-file line number

  int parseSaveField();
  int parseAddChar(Uchar32 uc);
  int parsing(Uchar32 uc);
};

}  // namespace csv

#endif
