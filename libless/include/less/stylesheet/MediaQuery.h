#ifndef __less_stylesheet_MediaQuery_h__
#define __less_stylesheet_MediaQuery_h__

#include "less/stylesheet/Selector.h"
#include "less/stylesheet/Stylesheet.h"
#include "less/stylesheet/StylesheetStatement.h"

class MediaQuery : public Stylesheet, public StylesheetStatement {
private:
  Selector selector;

public:
  Selector& getSelector();
  void setSelector(const Selector& s);

  MediaQuery* createMediaQuery() override;

  void process(Stylesheet& s) override;
  void write(CssWriter& writer) override;
};

#endif  // __less_stylesheet_MediaQuery_h__
