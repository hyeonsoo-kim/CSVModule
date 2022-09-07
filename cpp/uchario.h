#ifndef _CSVMODULE_UCHARIO_H_
#define _CSVMODULE_UCHARIO_H_

#include <unicode/uchar.h>    // Uchar
#include <unicode/ucnv.h>     // UConverter
#include <unicode/uobject.h>  // RTTI for Unicode Objects
#include <unicode/utf16.h>    // UTF16 Unicode data
#include <unicode/utypes.h>   // Unicide data types

#include <iostream>

#define TRUE 1
#define FALSE 0

/*
 * I/O Stream Operator for single UChar(Unicode Character)
 * Referenced to icu/io/ustream.cpp
 (https://github.com/unicode-org/icu/blob/main/icu4c/source/io/ustream.cpp) */

/*
 * Operator>> (Input : Extract Operator)
 * @brief
 * read Bytes and convert single unicode character.
 * @param stream input steam object ( i.e. std::istream, std::ifstream,
 * std::stringstream)
 * @param res UChar
 * @return reference of input stream used parameter.
 */

std::istream& operator>>(std::istream& stream, UChar& res);

std::ostream& operator<<(std::ostream& stream, UChar& target);

#endif
