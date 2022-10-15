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
#include "uchario.h"

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
enum QuoteStyle { QUOTE_MINIMAL, QUOTE_ALL, QUOTE_NONNUMERIC, QUOTE_NONE };

struct Dialect {
  bool doublequote;
  bool skipinitialspace;
  bool strict;
  QuoteStyle quoting;
  UChar delimiter;
  UChar quotechar;
  UChar escapechar;
  icu::UnicodeString lineterminator;

  Dialect()
      : doublequote(true),
        skipinitialspace(false),
        strict(false),
        quoting(QUOTE_NONE),
        delimiter(u','),
        quotechar(u'\"'),
        escapechar(0),
        lineterminator(icu::UnicodeString("\r\n")) {}
};

class CSVReader {
 public:
  CSVReader(const char* filename)
      : file(std::ifstream(filename)),
        state(START_RECORD),
        line_num(0),
        dialect(Dialect()) {
    UChar c;
    while (!file.eof()) {
      do {
        file >> std::noskipws >> c;
        parsing(c);
        if (state == MATCH_NEWLINE || file.eof()) {
          state = START_RECORD;
        }
      } while (state != START_RECORD);
      data.append(currfields);
      currfields.clear();
    }
  }

  ArrayList<ArrayList<icu::UnicodeString> > getdata() const { return data; }

 private:
  std::ifstream file;                              // input filestream
  ArrayList<ArrayList<icu::UnicodeString> > data;  // List of Datas
  ParserState state;                               // current CSV parse state
  ArrayList<icu::UnicodeString> currfields;        // fields in current line
  icu::UnicodeString fieldbuf;                     // temporary buffer
  std::size_t line_num;                            // source-file line number
  Dialect dialect;  // CSV Dialect : setting of csv structure
  bool numeric_field;

  int parseSaveField();
  int parseAddChar(UChar uc);
  int parsing(UChar uc);
};

}  // namespace csv

#endif
