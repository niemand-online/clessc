#include <utility>

#include "less/css/ParseException.h"

using namespace std;

ParseException::ParseException(string found,
                               string& expected,
                               unsigned int line,
                               unsigned int column,
                               string source) {
  err.append("Found \"");
  err.append(translate(std::move(found)));
  err.append("\" when expecting ");
  err.append(expected);
  setLocation(line, column);
  this->source = std::move(source);
}

ParseException::ParseException(string found,
                               const char* expected,
                               unsigned int line,
                               unsigned int column,
                               string source) {
  err.append("Found \"");
  err.append(translate(std::move(found)));
  err.append("\" when expecting ");
  err.append(expected);
  setLocation(line, column);
  this->source = std::move(source);
}
ParseException::ParseException(const char* found,
                               const char* expected,
                               unsigned int line,
                               unsigned int column,
                               string source) {
  err.append("Found \"");
  if (found[0] == -1)
    err.append("end of file");
  else
    err.append(translate(string(found)));
  err.append("\" when expecting ");
  err.append(expected);
  setLocation(line, column);
  this->source = std::move(source);
}
ParseException::ParseException(Token& found, const char* expected) {
  err.append("Found \"");
  err.append(translate(found));
  err.append("\" when expecting ");
  err.append(expected);
  setLocation(found.line, found.column);
  this->source = found.source;
}
ParseException::ParseException(TokenList& found, const char* expected) {
  err.append("Found \"");
  err.append(translate(found.toString()));
  err.append("\" when expecting ");
  err.append(expected);
  setLocation(found.front().line, found.front().column);
  this->source = found.front().source;
}
void ParseException::setLocation(unsigned int line, unsigned int column) {
  this->line = line;
  this->column = column;
}

unsigned int ParseException::getLineNumber() const {
  return line;
}
unsigned int ParseException::getColumn() const {
  return column;
}

/**
 * URL or file name where the Less code is located.
 */
void ParseException::setSource(string source) {
  this->source = std::move(source);
}

string ParseException::getSource() const {
  return source;
}

const char* ParseException::what() const throw() {
  return err.c_str();
}

string ParseException::translate(string found) {
  if (found == "\n")
    return "newline";
  return found;
}
