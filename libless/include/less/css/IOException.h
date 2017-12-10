#ifndef __less_css_IOException_h__
#define __less_css_IOException_h__

#include <exception>

class IOException : public std::exception {
public:
  const char* err;

  IOException(std::string& err) {
    this->err = err.c_str();
  }

  IOException(const char* err) {
    this->err = err;
  }

  IOException(const IOException&) = default;
  IOException(IOException&&) = default;

  IOException& operator=(const IOException&) = default;
  IOException& operator=(IOException&&) = default;

  virtual const char* what() const throw() {
    return err;
  }
};

#endif  // __less_css_IOException_h__
