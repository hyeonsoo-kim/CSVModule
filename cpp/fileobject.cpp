#include "fileobject.h"
#define EOL 0xfffe

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

int csv::CSVReader::parseAddChar(UChar uc) {
  int res = 0;
  try {
    fieldbuf += uc;
  } catch (std::exception& e) {
    res = -1;
  }
  return res;
}

int csv::CSVReader::parsing(UChar uc) {
  // TODO : Set Dialect.

  switch (state) {
    case START_RECORD:
      if (uc == EOL) {
        break;
      } else if (uc == u'\n' || uc == u'\r') {
        state = MATCH_NEWLINE;
        break;
      }
      state = START_FIELD;
    case START_FIELD:
      if (uc == u'\n' || uc == u'\r' || uc == EOL) {
        if (parseSaveField() < 0) {
          return -1;
        }
        state = (uc == EOL ? START_FIELD : MATCH_NEWLINE);
      } else if (uc == this->dialect.quotechar &&
                 this->dialect.quoting != QUOTE_NONE) {
        state = IN_QUOTED_FIELD;
      } else if (uc == this->dialect.escapechar) {
        state = ESCAPED_CHAR;
      } else if (uc == u' ' && this->dialect.skipinitialspace)
        ;
      else if (uc == this->dialect.delimiter) {
        if (parseSaveField() < 0) return -1;
      } else {
        if (this->dialect.quoting == QUOTE_NONNUMERIC) numeric_field = true;

        if (parseAddChar(uc) < 0) {
          return -1;
        }
        state = IN_FIELD;
      }
      break;
    case ESCAPED_CHAR:
      if (uc == u'\r' || uc == u'\n') {
        if (parseAddChar(uc) < 0) {
          return -1;
        }
        state = AFTER_ESCAPED_NEWLINE;
        break;
      }
      if (uc == EOL) {
        uc = u'\n';
      }
      if (parseAddChar(uc) < 0) {
        return -1;
      }
      state = IN_FIELD;
      break;
    case AFTER_ESCAPED_NEWLINE:
      if (uc == EOL) break;

    case IN_FIELD:
      if (uc == EOL || uc == u'\r' || uc == u'\n') {
        if (parseSaveField() < 0) return -1;
        state = (uc == EOL ? START_RECORD : MATCH_NEWLINE);
      } else if (uc == this->dialect.escapechar) {
        state = ESCAPED_CHAR;
      } else if (uc == this->dialect.delimiter) {
        if (parseSaveField() < 0) return -1;
        state = START_FIELD;
      } else {
        if (parseAddChar(uc) < 0) return -1;
      }
      break;

    case IN_QUOTED_FIELD:
      if (uc == EOL)
        ;
      else if (uc == this->dialect.escapechar) {
        state = ESCAPE_IN_QUOTED_FIELD;
      } else if (uc == this->dialect.quotechar &&
                 this->dialect.quoting != QUOTE_NONE) {
        if (this->dialect.doublequote) {
          state = QUOTE_IN_QUOTED_FIELD;
        } else {
          state = IN_FIELD;
        }
      } else {
        if (parseAddChar(uc) < 0) return -1;
      }
      break;

    case ESCAPE_IN_QUOTED_FIELD:
      if (uc == EOL) uc == u'\n';
      if (parseAddChar(uc) < 0) return -1;
      state = IN_QUOTED_FIELD;
      break;

    case QUOTE_IN_QUOTED_FIELD:
      if (this->dialect.quoting != QUOTE_NONE &&
          uc == this->dialect.quotechar) {
        if (parseAddChar(uc) < 0) return -1;
        state = IN_QUOTED_FIELD;
      } else if (uc == this->dialect.delimiter) {
        if (parseSaveField() < 0) return -1;
        state = START_FIELD;
      } else if (uc == u'\n' || uc == u'\r' || uc == EOL) {
        if (parseSaveField() < 0) return -1;
        state = (uc == EOL ? START_RECORD : MATCH_NEWLINE);
      } else if (!this->dialect.strict) {
        if (parseAddChar(uc) < 0) return -1;
        state = IN_FIELD;
      } else {
        // Error
        return -1;
      }
      break;

    case MATCH_NEWLINE:
      if (uc == u'\r' || uc == u'\n')
        // data.append(currfields);
        ;
      else if (uc == EOL)
        state = START_RECORD;
      else {
        // ERROR
        return -1;
      }
      break;
  }
  return 0;
}
