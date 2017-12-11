#ifndef __less_value_ValueException_h__
#define __less_value_ValueException_h__

#include <string>
#include "less/TokenList.h"

/**
 *
 */
class ValueException : public std::exception {
public:
  std::string err;
  const char* _source;
  unsigned int line, column;

  ValueException(std::string message, const TokenList& source) {
    err = message;
    this->_source = source.front().source;
    line = source.front().line;
    column = source.front().column;
  }
  ~ValueException() throw() override{};

  ValueException(const ValueException&) = default;
  ValueException(ValueException&&) = default;

  ValueException& operator=(const ValueException&) = default;
  ValueException& operator=(ValueException&&) = default;

  const char* what() const throw() override {
    return err.c_str();
  }

  unsigned int getLineNumber() {
    return line;
  }
  unsigned int getColumn() {
    return column;
  }
  const char* getSource() {
    return _source;
  }
};

#endif  // __less_value_ValueException_h__
