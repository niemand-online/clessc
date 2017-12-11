#ifndef __less_stylesheet_AtRule_h__
#define __less_stylesheet_AtRule_h__

#include "less/stylesheet/Stylesheet.h"
#include "less/stylesheet/StylesheetStatement.h"

#include "less/css/CssWriter.h"

#include "less/Token.h"
#include "less/TokenList.h"

class AtRule : public StylesheetStatement {
private:
  Token keyword;
  TokenList rule;

public:
  AtRule(const Token &keyword);
  ~AtRule() override = default;
  void setKeyword(const Token &keyword);
  void setRule(const TokenList &rule);

  Token &getKeyword();
  TokenList &getRule();

  void process(Stylesheet &s) override;
  void write(CssWriter &writer) override;
};

#endif  // __less_stylesheet_AtRule_h__
