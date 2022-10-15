#include "uchario.h"

std::istream& operator>>(std::istream& stream, UChar& res) {
  // if stream is fail, return stream and work nothing.
  if (stream.fail()) {
    return stream;
  }

  // prepare buffer to extract Bytes and convert
  char buffer[16];  // buffer for raw bytes.
  int32_t idx = 0;  // index of raw bytes buffer

  UChar Ubuffer[1];  // buffer for single unicode character
  // Ubuffer[1] = 0;
  UConverter* converter;  // Object for Unicode conversion
  UErrorCode errstate =
      U_ZERO_ERROR;  // Object for error state during Unicode conversion

  // Open(make) Unicode converter
  converter = ucnv_open(NULL, &errstate);

  // If Unicode converter is made succeccfully, read bytes and convert to
  // Unicode.
  if (U_SUCCESS(errstate)) {
    UChar* UStart = Ubuffer;              // start of UBuffer
    const UChar* ULimit = Ubuffer + 2UL;  // End of UBuffer
    const char *b, *bLimit;
    char byte;
    UChar result;

    UBool initialWhitespace = TRUE;
    UBool continueExtractBytes = TRUE;

    while (continueExtractBytes) {
      byte = stream.get();
      if (stream.eof()) {
        if (!initialWhitespace) {
          stream.clear(stream.eofbit);
        }
        continueExtractBytes = FALSE;
      }

      bLimit = &byte + static_cast<int>(continueExtractBytes);
      UStart = Ubuffer;
      b = &byte;
      errstate = U_ZERO_ERROR;

      // convert bytes to unicode
      ucnv_toUnicode(converter, &UStart, ULimit, &b, bLimit, 0,
                     !continueExtractBytes, &errstate);

      // if conversion failed. stop.
      if (U_FAILURE(errstate)) {
        stream.clear(stream.failbit);
        goto STOP;
      }

      // If conversion is completed. extract unicode character.
      if (UStart != Ubuffer) {
        int32_t offset = static_cast<int32_t>(UStart - Ubuffer);
        int32_t UBufIdx = 0;
        U16_NEXT(Ubuffer, UBufIdx, offset, result);
        if (u_isWhitespace(result)) {
          if (!initialWhitespace) {
            buffer[idx++] = byte;
            while (idx > 0) {
              stream.putback(buffer[--idx]);
            }
            goto STOP;
          }
          // TODO: read whitespace charcter if stream's flag setted noskipws'
          if (!(stream.flags() & 4096)) {
            res = result;
            goto STOP;
          }
        } else {
          if (initialWhitespace) {
            initialWhitespace = FALSE;
          }
          res = result;
          goto STOP;
        }
        idx = 0;
      } else {
        buffer[idx++] = byte;
      }
    }
  STOP:
    ucnv_close(converter);
  }
  return stream;
}

std::ostream& operator<<(std::ostream& stream, UChar& target) {
  char buffer[16] = {
      0,
  };
  UConverter* converter;
  UErrorCode errstate = U_ZERO_ERROR;
  UChar Uch = target;

  converter = ucnv_open(NULL, &errstate);
  if (U_SUCCESS(errstate)) {
    const UChar* us = &Uch;
    const UChar* uLimit = us + 1;

    char *s, *sLimit = buffer + (sizeof(buffer) - 1);

    s = buffer;
    ucnv_fromUnicode(converter, &s, sLimit, &us, uLimit, 0, false, &errstate);
    if (s > buffer) {
      stream << buffer;
    }
    ucnv_close(converter);
  }

  return stream;
}
