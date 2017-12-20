#ifndef __less_stylesheet_CssComment_h__
#define __less_stylesheet_CssComment_h__

#include "less/Token.h"
#include "less/css/CssWriter.h"
#include "less/stylesheet/RulesetStatement.h"
#include "less/stylesheet/Stylesheet.h"
#include "less/stylesheet/StylesheetStatement.h"

class CssComment : public StylesheetStatement, public RulesetStatement {
protected:
  Token comment;

public:
  CssComment();
  explicit CssComment(const Token &comment);

  void setComment(const Token &comment);
  Token &getComment();

  void process(Ruleset &r) override;
  void process(Stylesheet &s) override;

  void write(CssWriter &writer) override;
};

#endif  // __less_stylesheet_CssComment_h__
