#ifndef __less_css_ParseException_h__
#define __less_css_ParseException_h__

#include <exception>
#include <string>

#include "less/Token.h"
#include "less/TokenList.h"

class ParseException : public std::exception {
public:
  std::string err;

  std::string source;
  unsigned int line, column;

  ParseException(std::string found,
                 std::string& expected,
                 unsigned int line,
                 unsigned int column,
                 std::string source);
  ParseException(std::string found,
                 const char* expected,
                 unsigned int line,
                 unsigned int column,
                 std::string source);
  ParseException(const char* found,
                 const char* expected,
                 unsigned int line,
                 unsigned int column,
                 std::string source);

  ParseException(Token& found, const char* expected);
  ParseException(TokenList& found, const char* expected);

  ParseException(const ParseException&) = default;
  ParseException(ParseException&&) = default;

  ParseException& operator=(const ParseException&) = default;
  ParseException& operator=(ParseException&&) = default;

  ~ParseException() throw() override{};

  void setLocation(unsigned int line, unsigned int column);
  unsigned int getLineNumber();
  unsigned int getColumn();

  /**
   * URL or file name where the Less code is located.
   */
  void setSource(std::string source);

  std::string getSource();
  const char* what() const throw() override;

protected:
  std::string translate(std::string found);
};

#endif  // __less_css_ParseException_h__
