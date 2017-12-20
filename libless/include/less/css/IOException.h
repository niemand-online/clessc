#ifndef __less_css_IOException_h__
#define __less_css_IOException_h__

#include <exception>

class IOException : public std::exception {
public:
  const char* err;

  explicit IOException(std::string& err) {
    this->err = err.c_str();
  }

  explicit IOException(const char* err) {
    this->err = err;
  }

  IOException(const IOException&) = default;
  IOException(IOException&&) = default;

  IOException& operator=(const IOException&) = default;
  IOException& operator=(IOException&&) = default;

  const char* what() const throw() override {
    return err;
  }
};

#endif  // __less_css_IOException_h__
