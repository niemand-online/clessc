#ifndef __less_less_LessTokenizer_h__
#define __less_less_LessTokenizer_h__

#include <iostream>
#include "less/css/CssTokenizer.h"

/**
 * Extends CssTokenizer:
 *  - Add support for c++ style comments
 *
 */
class LessTokenizer : public CssTokenizer {
public:
  LessTokenizer(std::istream& in, const char* source)
      : CssTokenizer(in, source){};
  virtual ~LessTokenizer();

protected:
  bool readComment() override;
};

#endif  // __less_less_LessTokenizer_h__
