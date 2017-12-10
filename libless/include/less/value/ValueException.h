#ifndef __less_value_ValueException_h__
#define __less_value_ValueException_h__

#include <string>
#include "less/TokenList.h"

using namespace std;
/**
 *
 */
class ValueException : public exception {
public:
  string err;
  const char* _source;
  unsigned int line, column;

  ValueException(string message, const TokenList& source) {
    err = message;
    this->_source = source.front().source;
    line = source.front().line;
    column = source.front().column;
  }
  virtual ~ValueException() throw(){};

  ValueException(const ValueException&) = default;
  ValueException(ValueException&&) = default;

  ValueException& operator=(const ValueException&) = default;
  ValueException& operator=(ValueException&&) = default;

  virtual const char* what() const throw() {
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
